#ifndef TrackingFeedbackProcessor_h
#define TrackingFeedbackProcessor_h 1

#include <string>

#include "marlin/Processor.h"
#include "lcio.h"
#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCRelation.h"
#include "EVENT/Track.h"

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include "TrueTrack.h"
#include "RecoTrack.h"



using namespace lcio ;
using namespace marlin ;


/**  Feedback for the efficiency and the ghostrate of the tracking.
 * 
 * 
 *  <h4>Input - Prerequisites</h4>
 *  A collection of reconstructed tracks and a relation collection that relates true tracks with monte carlo particles
 *
 *  <h4>Output</h4> 
 *  Results will be output to screen (depending of debug level), to a csv file and to a root file 
 * 
 * @param TrackCollection Name of Track collection to check <br>
 * (default value ForwardTracks )
 * 
 * @param MCTrueTrackRelCollectionName Name of the TrueTrack MC Relation collection<br>
 * (default value TrueTracksMCP )
 * 
 * @param TableFileName Name of the table file for saving the results <br>
 * (default value TrackingFeedback.csv )
 * 
 * @param SaveAllEventsSummary If true the results of all events are summed up and saved in the file specified under SummaryFileName <br>
 * (default value false )
 * 
 * @param SummaryFileName All events are summed up and saved in this file, if SaveAllEventsSummary == true <br>
 * (default value TrackingFeedbackSum.csv )
 * 
 * @param MultipleScatteringOn Whether to take multiple scattering into account when fitting the tracks<br>
 * (default value true )
 * 
 * @param EnergyLossOn Whether to take energy loss into account when fitting the tracks<br>
 * (default value true )
 * 
 * @param SmoothOn Whether to smooth all measurement sites in fit<br>
 * (default value false )
 * 
 * @param CutPtMin The minimum transversal momentum pt above which tracks are of interest in GeV <br>
 * (default value 0.1 )
 * 
 * @param CutDistToIPMax The maximum distance from the origin of the MCP to the IP (0,0,0) <br>
 * (default value 10000 )
 * 
 * @param CutChi2Prob Tracks with a chi2 probability below this value won't be considered <br>
 * (default value 0.005 )
 * 
 * @param CutNumberOfHitsMin The minimum number of hits a track must have <br>
 * (default value 4 )
 * 
 * @param CutNumberOfHitsMin_HitsCountOncePerLayer Whether the hits used for the cut CutNumberOfHitsMin only count 
 * once for each layer, i.e double hits on a layer count as one <br>
 * (default value false, i.e. every hit counts )
 * 
 * @param CutThetaMin The minimum theta of the track in deg
 * (default value 0 )
 * 
 * @param CutThetaMax The maximum theta of the track in deg
 * (default value 91 )
 * 
 * @param CutFitFails Whether to cut all tracks that fail at fitting
 * (default value false )
 * 
 * @param DrawMCPTracks Draw the helices of the MCP (values at IP) in CED <br>
 * (default value false )
 * 
 * @param RateOfFoundHitsMin More than this rate of hits of the real track must be in a reco track to be assigned <br>
 * (default value 0.5 )
 * 
 * @param RateOfAssignedHitsMin More than this rate of hits of the reco track must belong to the true track to be assigned <br>
 * (default value 0.5 )
 * 
 * @param RootFileName Name for the root file where the tracks are saved<br>
 * (default value "Feedback.root")
 * 
 * @param RootFileAppend Whether the root output file should be appended to an existing one<br>
 * (default value false)
 * 
 * @author Robin Glattauer HEPHY, Wien
 *
 */
class TrackingFeedbackProcessor : public Processor {
  
 public:
  
    virtual Processor*  newProcessor() { return new TrackingFeedbackProcessor ; }
  
  
    TrackingFeedbackProcessor() ;
  
  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;
  
  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;
  
  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ; 
  
  
  virtual void check( LCEvent * evt ) ; 
  
  
  /** Called after data processing for clean up.
  */
  virtual void end() ;
  
  
 protected:

   
   double _Bz; //B field in z direction
   
   
   /** Input collection name.
   */
   std::string _colNameMCTrueTracksRel;
   std::string _TrackCollection;

   
   std::string _tableFileName;


   int _nRun ;
   int _nEvt ;
   
   double _cutPtMin;
   double _cutDistToIPMax;
   double _cutChi2Prob;
   int    _cutNHitsMin;
   bool _cutNHitsMin_HitsCountOncePerLayer;
   double _cutThetaMin;
   double _cutThetaMax;
   bool _cutFitFails;

   
   bool _MSOn ;
   bool _ElossOn ;
   bool _SmoothOn ;
   
   
   
   unsigned _nComplete;            // complete tracks without extra points
   unsigned _nCompletePlus;        // complete tracks with extra points
   unsigned _nLost;                // lost tracks = tracks that do exist in reality (mcp), but are not represented in the tracksearch results  
   unsigned _nIncomplete;          // incomplete tracks without extra points. i.e.: tracks that are too short (for example 1 or 2 hits are still missing)
   unsigned _nIncompletePlus;      // incomplete tracks with extra points. the reconstructed track belongs to the true track that hold more than half of the points of the track
   unsigned _nGhost;               // ghost tracks = tracks that are reconstructed, but don't actually exist. Pure fiction. a ghost track 
                                   // is a track, where no real track owns more than half of the tracks hits.
   unsigned _nFoundCompletely;     // tracks have been found that contain all the hits of this track
   unsigned _nRecoTracks;          // the number of reconstructed tracks, that were compared to the true tracks
   unsigned _nDismissedTrueTracks; // number of the true tracks, that haven't been used
   unsigned _nValidTrueTracks;     // number of valid true tracks, i.e. true tracks not failing the cuts
   unsigned _nClones;              // the number of tracks that are reconstructed although there already is a reconstruction
                                   // --> The number of clones for a true track is nReco - 1, or 0 if there are no reco tracks
   
   unsigned _nComplete_Sum;            
   unsigned _nCompletePlus_Sum;       
   unsigned _nLost_Sum;               
   unsigned _nIncomplete_Sum;         
   unsigned _nIncompletePlus_Sum;   
   unsigned _nGhost_Sum;            
   unsigned _nFoundCompletely_Sum;     
   unsigned _nRecoTracks_Sum;          
   unsigned _nDismissedTrueTracks_Sum; 
   unsigned _nValidTrueTracks_Sum;
   unsigned _nClones_Sum;
   
   
   std::vector< TrueTrack* > _trueTracks;
   std::vector< RecoTrack* > _recoTracks;
   
   bool _drawMCPTracks;
   bool _saveAllEventsSummary;
   std::string _summaryFileName;
  
   
  
   
   void checkTheTrack( RecoTrack* recoTrack );
   TrueTrack* getAssignedTrueTrack( std::vector<TrueTrack*> relatedTrueTracks , unsigned& nHitsFromAssignedTrueTrack );
   
   unsigned getNumberOfHitsFromDifferentLayers( std::vector< TrackerHit* > hits );
   double getDistToIP( MCParticle* mcp );
   
   MarlinTrk::IMarlinTrkSystem* _trkSystem;
   
   TTree * _treeTrueTracks;
   TTree * _treeRecoTracks;
   TFile * _rootFile;
   std::string _rootFileName;
   bool _rootFileAppend;
   std::string _treeNameTrueTracks;
   std::string _treeNameRecoTracks;
   
   
   void saveRootInformation();   
   void makeRootBranches();
   void setRootBranches();
   

   float _rateOfFoundHitsMin;  //more than this number of hits of the real track must be in the reco track
   float _rateOfAssignedHitsMin;  //more than this number of hits of the reco track must belong to the assigned true track
   
   
   
   int _trueTrack_nComplete;
   int _trueTrack_nCompletePlus;
   int _trueTrack_nIncomplete;
   int _trueTrack_nIncompletePlus;
   int _trueTrack_nSum;
   double _trueTrack_pt;
   double _trueTrack_theta;
   int _trueTrack_nHits;
   double _trueTrack_vertexX;
   double _trueTrack_vertexY;
   double _trueTrack_vertexZ;
   double _trueTrack_chi2prob;
   double _trueTrack_chi2;
   int _trueTrack_Ndf;
   
   int _recoTrack_nTrueTracks;
   double _recoTrack_pt;
   double _recoTrack_chi2prob;
   double _recoTrack_chi2;
   int _recoTrack_Ndf;
   
   
} ;

#endif



