import FWCore.ParameterSet.Config as cms

process = cms.Process("LLP")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Mu/AOD/Apr21ReReco-v1/0000/00459D48-EB70-E011-AF09-90E6BA19A252.root"
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("output.root")
)


process.analyzer = cms.EDAnalyzer('TrackAnalyzer',
                                  tracks = cms.InputTag("generalTracks"),
                                  vertices = cms.InputTag("offlinePrimaryVertices"),
                                  met = cms.InputTag("pfMet")
                                  )

process.p = cms.Path(process.analyzer)
