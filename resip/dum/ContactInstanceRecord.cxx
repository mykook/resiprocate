#include "resip/dum/ContactInstanceRecord.hxx"
#include "rutil/Timer.hxx"
#include "resip/stack/SipMessage.hxx"

using namespace resip;

ContactInstanceRecord::ContactInstanceRecord() : 
   mRegExpires(0),
   mLastUpdated(Timer::getTimeSecs()),
   mRegId(0),
   mSyncContact(false),
   mUserInfo(0)
{
}

bool
ContactInstanceRecord::operator==(const ContactInstanceRecord& rhs) const
{
   if((mRegId != 0 && !mInstance.empty()) || 
      (rhs.mRegId != 0 && !rhs.mInstance.empty()))
   {
      // If regId and instanceId is specified on either, then outbound RFC5626 is 
      // in use - match only if both instance id and reg-id match - ignore contact URI
      return mInstance == rhs.mInstance && 
             mRegId == rhs.mRegId;
   }
   else
   {
      // otherwise both instance (if specified) and contact must match
      return mInstance == rhs.mInstance &&
             mContact.uri() == rhs.mContact.uri();
   }
}

ContactInstanceRecord 
ContactInstanceRecord::makeRemoveDelta(const NameAddr& contact)
{
   ContactInstanceRecord c;
   c.mContact = contact;
   return c;
}

ContactInstanceRecord 
ContactInstanceRecord::makeUpdateDelta(const NameAddr& contact, 
                                       UInt64 expires,  // absolute time in secs
                                       const SipMessage& msg)
{
   ContactInstanceRecord c;
   c.mContact = contact;
   c.mRegExpires = expires;
   c.mReceivedFrom = msg.getSource();
   if (msg.exists(h_Paths))
   {
      c.mSipPath = msg.header(h_Paths);
   }
   if (contact.exists(p_Instance))
   {
      c.mInstance = contact.param(p_Instance);
   }
   if (contact.exists(p_regid))
   {
      c.mRegId = contact.param(p_regid);
   }
   // !jf! need to fill in mServerSessionId here
   return c;
  
}

      
