/*
 * This project is a little challenge: the goal is to find the reason for the crash by using
 * only the "Release" configuration. You will probably need to find the "this" pointer at the 
 * time of the crash.
 */

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <Windows.h>

class RandomMessagePrinter {
public:
	virtual void printMessage() = 0;

protected:
	int nextRandomValue() const
	{		
		if (!generatorSeeded)
		{
			srand(static_cast<int>(time(NULL)));
			generatorSeeded = true;
		}

		return rand();
	}
private:
	mutable bool generatorSeeded = false;
};

class FortuneCookie : public RandomMessagePrinter
{
public:	
	FortuneCookie()
	{
		setMessagesOffset(1);
	}

	virtual void printMessage()
	{
		int messageIndex = -1;
		while (messageIndex == -1 || this->messageAlreadyPrinted(messageIndex))
		{
			messageIndex = this->getNextMessageIndex();
		}		
		if (messageIndex != -1)
		{
			const char* messageToPrint = MESSAGES[messageIndex];
			OutputDebugString(messageToPrint);
			OutputDebugString("\n");
		}
		else
		{
			std::cerr << "Could not find message to print!" << std::endl;
		}
	}

private:
	void setMessagesOffset(int messagesOffset)
	{
		m_messagesOffset = messagesOffset;
	}

	bool messageAlreadyPrinted(int messageIndex) const
	{
		return std::find(	m_messagesAlreadyPrinted.begin(),
							m_messagesAlreadyPrinted.end(), 
							messageIndex) != m_messagesAlreadyPrinted.end();
	}

	int getNextMessageIndex() const
	{
		return this->nextRandomValue() + (m_messagesOffset == -1 ? 0 : m_messagesOffset);
	}

	void addMessageIndexToAlreadyPrintedMessages(int messageIndex)
	{
		if (m_messagesAlreadyPrinted.size() == NB_MESSAGES - 1)
		{
			m_messagesAlreadyPrinted.clear();
		}
		else
		{
			m_messagesAlreadyPrinted.push_back(messageIndex);
		}
	}

	static const unsigned int NB_MESSAGES = 2;
	static const char* const MESSAGES[NB_MESSAGES];
		
	std::vector<unsigned int> m_messagesAlreadyPrinted;
	int m_messagesOffset;
};

const char* const FortuneCookie::MESSAGES[NB_MESSAGES] =
{
	"Everybody feels lucky for having you as a friend.",
	"Good clothes open many doors. Go shopping."
};

int main()
{
	RandomMessagePrinter* messagePrinter = new FortuneCookie();
	messagePrinter->printMessage();

	return 0;
}
