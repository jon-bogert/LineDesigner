#pragma once

#include <functional>
#include <vector>

#ifndef XE_UNDOREDOSTACK_DEFAULT_SIZE
#define XE_UNDOREDOSTACK_DEFAULT_SIZE 50
#endif // XE_UNDOREDOSTACK_DEFAULT_SIZE

namespace xe
{
	struct Command
	{
		std::function<void(void)> execute;
		std::function<void(void)> revert;
	};

	class CommandStack
	{
	private:
		static const size_t k_undefined = SIZE_MAX;
	
	public:
		CommandStack(size_t size)
		{
			m_stack.resize(size);
		}

		CommandStack()
		{
			m_stack.resize(XE_UNDOREDOSTACK_DEFAULT_SIZE);
		}

		void PushAndExecute(const Command& cmd)
		{
			size_t index = 0;
			if (!IsEmpty() && m_cursorIndex != m_stack.size() - 1)
				index = m_cursorIndex + 1;

			if (index == m_bottomIndex && !IsEmpty()) // Check increase `bottomIndex`, but not if empty
				m_bottomIndex = (m_bottomIndex == m_stack.size() - 1) ? 0 : m_bottomIndex + 1;

			m_stack[index] = cmd;
			m_topIndex = m_cursorIndex = index;

			cmd.execute();
		}

		void PushAndExecute(const std::function<void(void)>& execute, const std::function<void(void)>& revert)
		{
			size_t index = 0;
			if (!IsEmpty() && m_cursorIndex != m_stack.size() - 1)
				index = m_cursorIndex + 1;

			if (index == m_bottomIndex && !IsEmpty()) // Check increase `bottomIndex`, but not if empty
				m_bottomIndex = (m_bottomIndex == m_stack.size() - 1) ? 0 : m_bottomIndex + 1;

			m_topIndex = m_cursorIndex = index;

			Command& cmd = m_stack[index];
			cmd = Command();
			cmd.execute = execute;
			cmd.revert = revert;

			cmd.execute();
		}

		void Undo()
		{
			if (IsEmpty() || IsPastBottom()) // Empty or at bottom
				return;

			m_stack[m_cursorIndex].revert();

			if (m_cursorIndex == m_bottomIndex)
				m_cursorIndex = k_undefined;
			else if (m_cursorIndex == 0)
				m_cursorIndex = m_stack.size() - 1;
			else
				--m_cursorIndex;
		}

		void Redo()
		{
			if (IsEmpty() || IsAtTop())
				return;

			size_t index = 0;
			if (m_cursorIndex == k_undefined)
				index = m_bottomIndex;
			else if (m_cursorIndex != m_stack.size() - 1)
				index = m_cursorIndex + 1;

			m_stack[index].execute();
			m_cursorIndex = index;
		}

		bool IsEmpty() const
		{
			return m_topIndex == k_undefined;
		}

		bool IsPastBottom() const
		{
			return !IsEmpty() && m_cursorIndex == k_undefined;
		}

		bool IsAtTop() const
		{
			return m_cursorIndex == m_topIndex;
		}

		void Clear()
		{
			m_topIndex = m_cursorIndex = k_undefined;
			m_bottomIndex = 0;
		}

	private:
		std::vector<Command> m_stack;

		size_t m_topIndex = k_undefined; // Index of the most recent command (ie. the end of "redo" action)
		size_t m_bottomIndex = 0;
		size_t m_cursorIndex = k_undefined; // Index of the current command (where we currently are in the stack. behind is undo, ahead is redo)
	};
}