from ert.enkf.plot import DataFetcher, PlotData, SampleListCollection, ObservationDataFetcher, RefcaseDataFetcher


class PlotDataFetcher(DataFetcher):
    def __init__(self, ert):
        super(PlotDataFetcher, self).__init__(ert)


    def fetchData(self):
        observations = ObservationDataFetcher(self.ert()).fetchData()
        refcase = RefcaseDataFetcher(self.ert()).fetchData()

        result = {}
        result["keys"] = []
        result["observation_keys"] = []
        """ @type: dict of (str, PlotData) """

        for key in refcase.sample_lists_keys:
            sample_list = refcase[key]
            """ @type: SampleList """

            if not key in result:
                result[key] = PlotData()
                result[key].name = key
                result["keys"].append(key)

            result[key].setRefcase(sample_list)

        for key in observations.sample_lists_keys:
            sample_list = observations[key]
            """ @type: SampleList """

            if not key in result:
                result[key] = PlotData()
                result[key].name = key
                result["keys"].append(key)

            result["observation_keys"].append(key)
            result[key].setObservations(sample_list)

        result["keys"] = sorted(result["keys"])
        result["observation_keys"] = sorted(result["observation_keys"])

        return result

