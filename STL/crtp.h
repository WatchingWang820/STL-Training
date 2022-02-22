#pragma once
#include <iostream>

namespace watching {
	template<typename T>
	class CRTPBase {
	public:
		void Test() { Cast()->Test(); }
	private:
		CRTPBase() = default;
		friend T;
		T* Cast() {
			return static_cast<T*>(this);
		}
	};

	class CRTPDerivedA :public CRTPBase<CRTPDerivedA> {
	public:
		void Test() { std::cout << "this crtp a" << std::endl; }
	};

	class CRTPDerivedB :public CRTPBase<CRTPDerivedB> {
	public:
		void Test() { std::cout << "this crtp b" << std::endl; }
	};

	template<typename T>
	void CRTPTest(CRTPBase<T>& op) {
		op.Test();
	}
}