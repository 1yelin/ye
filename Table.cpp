#include "Table.h"
#include <iostream>
#include "PushButton.h"

Table::Table(int row, int col) : BasicWidget(0, 0, 0, 0), m_rows(row), m_cols(col), m_curPage(0), m_maxPage(0), m_extraDate(0)
{
	m_prevBtn = new PushButton("上一页");
	m_nextBtn = new PushButton("下一页");
	m_firstBtn = new PushButton("第一页");
	m_lastBtn = new PushButton("结尾页");
}

Table::~Table()
{
	delete m_prevBtn;
	delete m_nextBtn;
	delete m_firstBtn;
	delete m_lastBtn;

}

void Table::setRowCount(int row)
{
	m_rows = row;
}

void Table::setColCount(int col)
{
	m_cols = col;
}

void Table::setHeader(const std::string& header)
{
	m_header = header;
	m_cols = std::count(m_header.begin(), m_header.end(), '\t') + 1;

	m_tw = ::textwidth("计算机1041");
	m_th = ::textheight(m_header.c_str());
	//求出文字的宽度和高度
	m_gridW = m_tw + 10;
	m_gridH = m_th + 10;

	m_w = m_gridW * 6;  //数据6原来 = m_cols
	m_h = m_gridH * (11 + 1);  //数据11原来=  m_rows


}

void Table::insertData(const std::string& data)
{
	m_datas.push_back(data);
	updatePage();
}

void Table::show()
{

	drawTalbeGrid();
	drawHeader();
	drawTableData();
}

void Table::drawTalbeGrid()
{
	//确定表格有多少行，多少列
	setlinecolor(BLACK);

	//画横线
	for (size_t i = 0; i < 10 + 1; i++)   //数据10原来=  m_rows
	{
		line(m_x, m_y + i * m_gridH, m_x + 6 * m_gridW, m_y + i * m_gridH);
	}

    //画竖线
	for (size_t i = 0; i < 6 + 1; i++)  // 数据6原来 = m_cols
	{
		line(m_x + i * m_gridW, m_y, m_x + i * m_gridW, m_y + 10 * m_gridH);
	}

	if (m_maxPage > 0)
	{
		drawButton();
	}

}

void Table::drawButton()
{
	static bool flag = false;
	if (!flag)
	{
		m_prevBtn->move(m_x, m_h + 20);
		m_nextBtn->move(m_prevBtn->x() + m_prevBtn->width(), m_prevBtn->y());
		m_firstBtn->move(m_nextBtn->x() + m_prevBtn->width(), m_nextBtn->y());
		m_lastBtn->move(m_firstBtn->x() + m_prevBtn->width(), m_firstBtn->y());
	}


	m_prevBtn->show();
	m_nextBtn->show();
	m_firstBtn->show();
	m_lastBtn->show();

	char str[30] = { 0 };
	sprintf_s(str, "第%d页/共%d页", m_curPage + 1, m_maxPage + 1);

	outtextxy(m_lastBtn->x() + m_lastBtn->width() + 50, m_lastBtn->y() + 10, str);
}


void Table::drawTableData()
{
	if (m_datas.size() == 0)
		return;

/*	//防止越界  这个方法失败了
	if (m_rows > m_datas.size() && m_datas.size() != 0)   //10原来是m_rows
	{
		m_rows = m_datas.size();    //m_datas.size()原来是10
	}
	*/



	int begPos = m_curPage * 10; //数据开始的位置  m_curPage * m_rows
	int endPos = m_curPage * 10 + 10; //数据结束的位置   m_curPage * m_rows + m_rows

	//如果是最后一页，只遍历剩下的数据
	if (m_curPage == m_maxPage)
	{
		endPos = begPos + m_extraDate;
	}


	for (size_t i = begPos, r = 0; i < endPos; i++, r++)    //行
	{
		const auto& line_data = split(m_datas[i]);   
		for (size_t k = 0; k < line_data.size(); k++)  //列
		{

			int tx = m_x + k * m_gridW + (m_gridW - ::textwidth(line_data[k].c_str())) / 2;
			int ty = m_y + r * m_gridH + 5;
			outtextxy(tx, ty, line_data[k].c_str());

		}
	}
}

void Table::drawHeader()
{
	setlinestyle(PS_SOLID, 2);
	::rectangle(m_x, m_y - 30, m_x + m_w, m_y);
	for (size_t i = 0; i < 6; i++)  //6原本是m_cols
	{
		line(m_x + i * m_gridW, m_y - 30, m_x + i * m_gridW, m_y);
	}

	setlinestyle(PS_SOLID, 1);

	//分割表头数据
	auto headers = split(m_header);
//	std::cout << headers.size() << std::endl;
	for (size_t i = 0, j = 0; j < headers.size(); i++, j = j + 2)  //原本没有j的,但是我这个好像tab键占了位置，所以+2输出
	{
		int spaceH = (m_gridW - ::textwidth(headers[j].c_str())) / 2;
		int spaceV = (64 - ::textwidth(headers[j].c_str())) / 2;  //这里的64原来是30
		outtextxy(m_x + i * m_gridW + spaceH, m_y - m_gridH + spaceV, headers[j].c_str());
	}
}

std::vector<std::string> Table::split(std::string str, char separatpr)
{
	std::vector<std::string> res;

	for (size_t pos = 0; pos != std::string::npos ; )
	{
		//查找分隔字符的位置
		pos = str.find(separatpr);
		//取出字符串 0-10
		res.push_back(str.substr(0, pos));
		//把剩下的字符串保存到str
		str = std::string(str.c_str() + pos + 1);

	}


	return res;
}

void Table::updatePage()
{
	if (m_rows == 0 && !m_datas.empty())
		m_rows = m_datas.size();
	if (m_cols == 0 && !m_datas.empty())
	{
		m_cols = m_datas.front().find("\t") + 1;
	}

	//datas 82
	//linedata 10
	//maxpage 8

	if (m_rows >= m_datas.size())
	{
		m_maxPage = 0;
		m_extraDate = m_datas.size();
	}
	else
	{
		m_maxPage = m_datas.size() / 10; //10原来是m_rows
		m_extraDate = m_datas.size() % 10;  //10原来是m_rows
	}

}

void Table::event()
{
	BasicWidget::event();

	m_prevBtn -> event();
	m_nextBtn ->event();
	m_firstBtn ->event();
	m_lastBtn ->event();

	if (m_prevBtn->isClicked())
	{
		if (m_curPage != 0)
		{
			m_curPage--;
		}
	}
	if (m_nextBtn->isClicked())
	{
		if (m_curPage != m_maxPage)
		{
			m_curPage++;
		}
	}
	if (m_firstBtn->isClicked())
	{
		m_curPage = 0;

	}
	if (m_lastBtn->isClicked())
	{
		m_curPage = m_maxPage;
	}
}
