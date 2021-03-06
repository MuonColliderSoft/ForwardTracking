#ifndef TrueTrack_h
#define TrueTrack_h


#include "EVENT/MCParticle.h"
#include "EVENT/Track.h"
#include "lcio.h"
#include "MarlinTrk/IMarlinTrkSystem.h"
#include "MarlinTrk/IMarlinTrack.h"

#include "RecoTrack.h"

using namespace lcio;


/** A class to make linking reconstructed tracks and true tracks easier.
 * 
 * It represents a true track, so i wrapps a Track* and a MCParticle* ( accessible by getTrack() and getMCP() ) 
 * and offers additional functionality. 
 */
class TrueTrack{
   
public:
   
   
   TrueTrack( Track* trueTrack , MCParticle* mcp , MarlinTrk::IMarlinTrkSystem* trkSystem):
   _trueTrack(trueTrack), _mcp(mcp), _trkSystem(trkSystem) {}
   
   /** @return the true track */
   Track* getTrueTrack() const { return _trueTrack; }
   
   /** @return the monte carlo particle of the true track */
   const MCParticle* getMCP() const { return _mcp; }
   
      /** @return the reco tracks related to the true track */
   std::vector< const RecoTrack* > getRecoTracks() const{ return _recoTracks; }
   
   
   void addRecoTrack( RecoTrack* recoTrack ){ _recoTracks.push_back( recoTrack ); }
   
   
   
   
   /** @return whether this true track is lost */  
   bool isLost() const;
   
   /** @return whether this true track was found completely (complete or complete_plus) */
   bool isFoundCompletely() const;
   
   /** @return whether a complete version (with no additional hits) was found */
   bool completeVersionExists() const;
   

   
   /** @return Info about the Monte Carlo Particle */
   std::string getMCPInfo() const;
   
   /** @return Info about the true Track */
   std::string getTrueTrackInfo() const;
   
   /** @return Info about all the tracks associated to the true track */
   std::string getRelatedTracksInfo() const;
   
   /** @return Info about the status of the track concerning if it was found or lost */
   std::string getFoundInfo() const;
   
   /** @return Info whether the true track was cutted and if yes, by what */
   std::string getCutInfo() const;
   
   /** @return number of linked reco tracks that have a certain type */
   unsigned getNumberOfTracksWithType( TrackType type ) const;   
   
   /** @return the cuts it didn't survive. If the vector is empty, the true track survived all cuts. */
   std::vector< std::string > getCuts() const {return _cuts; }
   
   /** Stores the cut, that the true track didn't survive  
    * @param cut A string describing the cut
    */
   void addCut( std::string cut ){ _cuts.push_back( cut ); }
   
private:
   
   Track* _trueTrack;
   MCParticle* _mcp;
   std::vector< const RecoTrack* > _recoTracks;
   std::vector< std::string > _cuts;
   
   MarlinTrk::IMarlinTrkSystem* _trkSystem; // for fitting
   
   
};


#endif

