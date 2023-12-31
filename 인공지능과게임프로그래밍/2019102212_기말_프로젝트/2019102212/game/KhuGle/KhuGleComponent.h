#pragma once

#include <vector>

class CKhuGleComponent
{
public:
	std::vector<CKhuGleComponent*> m_Children;

	CKhuGleComponent *m_Parent;

	CKhuGleComponent();
	virtual ~CKhuGleComponent();

	void AddChild(CKhuGleComponent *pChild);

	virtual void Render() = 0;
};

