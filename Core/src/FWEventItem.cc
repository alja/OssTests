// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEventItem
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Thu Jan  3 14:59:23 EST 2008
//

// system include files
#include <iostream>
#include <algorithm>
#include <exception>
#include <TClass.h>

#include "ROOT/REveDataCollection.hxx"
#include "ROOT/REveManager.hxx"
#include "ROOT/REveScene.hxx"


// user include files
#include "DataFormats/FWLite/interface/Event.h"
#include "FireworksWeb/Core/interface/FWEventItem.h"
#include "FireworksWeb/Core/interface/FWItemAccessorBase.h"
#include "FireworksWeb/Core/interface/fwLog.h"

#include "FireworksWeb/Core/src/FWGenericHandle.h"
//
//
// constructors and destructor
//
FWEventItem::FWEventItem(std::shared_ptr<FWItemAccessorBase> iAccessor,
                         const FWPhysicsObjectDesc& iDesc) :
   m_accessor(iAccessor),
   m_collection(0),
   m_name(iDesc.name()),
   m_type(iDesc.type()),
   m_purpose(iDesc.purpose()),
   m_displayProperties(iDesc.displayProperties()),
   m_moduleLabel(iDesc.moduleLabel()),
   m_productInstanceLabel(iDesc.productInstanceLabel()),
   m_processName(iDesc.processName()),

   m_event(nullptr)
{
   m_collection = new ROOT::Experimental::REveDataCollection();
   m_collection->SetName(iDesc.name());
   m_collection->SetItemClass((TClass*)iAccessor->modelType());
   m_collection->SetMainColor(iDesc.displayProperties().color());
   if (!iDesc.filterExpression().empty())
      m_collection->SetFilterExpr(iDesc.filterExpression().c_str());
   auto sl = ROOT::Experimental::gEve->GetScenes();
   auto cs = sl->FindChild("Collections");
   
   // AMT :: this is a temprary workaround for interface in REveDataCollection
   m_tmp_expr_workaround = iDesc.filterExpression();
   cs->AddElement(m_collection);
}
// FWEventItem::FWEventItem(const FWEventItem& rhs)
// {
//    // do actual copying here;
// }

FWEventItem::~FWEventItem()
{
}

//
// assignment operators
//
// const FWEventItem& FWEventItem::operator=(const FWEventItem& rhs)
// {
//   //An exception safe implementation is
//   FWEventItem temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void
FWEventItem::setEvent(const edm::EventBase* iEvent)
{
   m_event = iEvent;

   m_accessor->reset();

   data();
}

//
// const member functions
//
const void*
FWEventItem::data() const
{
   //lookup data if we don't already have it
   if (m_accessor->data())
      return m_accessor->data();

   m_errorMessage.clear();
   if (!m_event)
      return m_accessor->data();

   // Retrieve the data from the event.
   edm::InputTag tag(m_moduleLabel, m_productInstanceLabel, m_processName);
   edm::TypeWithDict type(*(m_type->GetTypeInfo()));
   edm::FWGenericHandle handle(type);
   try
   {
      m_event->getByLabel(tag, handle);
      setData(*handle);
   }
   catch (std::exception& iException)
   {
      if (!m_printedErrorThisEvent)
      {
         std::ostringstream s;
         s << "Failed to get " << m_name << " because \n" <<iException.what();
         m_errorMessage=s.str();
         m_printedErrorThisEvent = true;
      }
      return nullptr;
   }

   return m_accessor->data();
}

void
FWEventItem::setData(const edm::ObjectWithDict& iData) const
{
   m_accessor->setData(iData);

   m_collection->ClearItems();
   
   // std::cout <<"FWEventItem::setData size "<<m_accessor->size()<<std::endl;
   for (size_t i = 0; i < m_accessor->size(); ++i)
   {
      std::string cname = m_collection->GetName();
      auto len = cname.size();
      char end = cname[len-1];
      if (end == 's') {
         cname = cname.substr(0, len-1);
      }
      TString pname(Form("%s %2d",  cname.c_str(), (int)i));
      m_collection->AddItem( (void*)m_accessor->modelData(i), pname.Data(), pname.Data() );
   }
   m_collection->ApplyFilter();
}

const TClass*
FWEventItem::modelType() const
{
   return m_accessor->modelType();
}
/*
void
FWEventItem::getPrimaryData() const
{
   //if(0!=m_data) return;
   if(nullptr!=m_accessor->data()) return;
   this->data(*(m_type->GetTypeInfo()));
}
*/
/*
bool
FWEventItem::isCollection() const
{
   return m_accessor->isCollection();
}
*/

/*
const void*
FWEventItem::modelData(int iIndex) const
{
   // this could also be retrived from REveDataCollection
   getPrimaryData();
   return m_accessor->modelData(iIndex);
}
*/



const TClass*
FWEventItem::type() const
{
   return m_type;
}

const std::string&
FWEventItem::purpose() const
{
   return m_purpose;
}

const std::string&
FWEventItem::moduleLabel() const
{
   return m_moduleLabel;
}
const std::string&
FWEventItem::productInstanceLabel() const
{
   return m_productInstanceLabel;
}

const std::string&
FWEventItem::processName() const
{
   return m_processName;
}

const char*
FWEventItem::name() const
{
   return m_collection->GetCName();
}


const std::string&
FWEventItem::filterExpression() const
{
  return  m_tmp_expr_workaround;
}


const FWDisplayProperties&
FWEventItem::defaultDisplayProperties() const
{
   return m_displayProperties;
}

void
FWEventItem::proxyConfigChanged(bool k)
{
   assert(0 && "FWEventItem::proxyConfigChanged not implementes");
}
