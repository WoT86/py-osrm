#include "parameters/matchparameter_nb.h"

#include "engine/api/match_parameters.hpp"
#include "utility/param_utility.h"

#include <nanobind/nanobind.h>

namespace nb = nanobind;
using namespace nb::literals;

void init_MatchParameters(nb::module_& m) {
    using osrm::engine::api::RouteParameters;
    using osrm::engine::api::MatchParameters;

    nb::class_<MatchParameters, RouteParameters> mp(m, "MatchParameters");
        mp.doc() =  "MatchParameters is an object containing configurations for OSRM match requests.\n\n"
                    "Attributes:\n"
                    "   - timestamps (list of unsigned int): Timestamps for the input locations in seconds since UNIX epoch.\n"
                    "   - gaps (MatchParameters.GapsType): Allows the input track splitting based on huge timestamp gaps between points.\n"
                    "   - tidy (bool): Allows the input track modification to obtain better matching quality for noisy tracks.\n"
                    "   - RouteParameters (osrm.RouteParameters): Attributes from parent class.\n"
                    "Examples:\n"
                    "    >>> match_params = osrm.MatchParameters(\n"
                    "            coordinates = [(7.41337, 43.72956), (7.41546, 43.73077), (7.41862, 43.73216)],\n"
                    "            timestamps = [1424684612, 1424684616, 1424684620],\n"
                    "            gaps = MatchParameters.GapsType.split,\n"
                    "            tidy = True\n"
                    "        )\n"
                    "    >>> match_params.IsValid()\n"
                    "    True;\n";

        nb::enum_<MatchParameters::GapsType>(mp, "GapsType")
            .value("split", MatchParameters::GapsType::Split)
            .value("ignore", MatchParameters::GapsType::Ignore);

        mp.def(nb::init<>())
        .def("__init__", [](MatchParameters* t,
                std::vector<unsigned> timestamps,
                MatchParameters::GapsType gaps_type,
                bool tidy,
                bool steps,
                int number_of_alternatives,
                const std::vector<RouteParameters::AnnotationsType>& annotations,
                RouteParameters::GeometriesType geometries,
                RouteParameters::OverviewType overview,
                const std::optional<bool> continue_straight,
                std::vector<std::size_t> waypoints,
                std::vector<osrm::util::Coordinate> coordinates,
                std::vector<std::optional<osrm::engine::Hint>> hints,
                std::vector<std::optional<double>> radiuses,
                std::vector<std::optional<osrm::engine::Bearing>> bearings,
                const std::vector<std::optional<osrm::engine::Approach>>& approaches,
                bool generate_hints,
                std::vector<std::string> exclude,
                const BaseParameters::SnappingType snapping
            ) {
                new (t) MatchParameters();

                t->timestamps = std::move(timestamps);
                t->gaps = gaps_type;
                t->tidy = tidy;

                osrm_nb_util::assign_routeparameters(t,
                                                     steps,
                                                     number_of_alternatives,
                                                     annotations,
                                                     geometries,
                                                     overview,
                                                     continue_straight,
                                                     waypoints);

                osrm_nb_util::assign_baseparameters(t,
                                                    coordinates,
                                                    hints,
                                                    radiuses,
                                                    bearings,
                                                    approaches,
                                                    generate_hints,
                                                    exclude,
                                                    snapping);
            },
                "timestamps"_a = std::vector<unsigned>(),
                "gaps"_a = MatchParameters::GapsType::Split,
                "tidy"_a = false,
                "steps"_a = false,
                "number_of_alternatives"_a = 0,
                "annotations"_a = std::vector<RouteParameters::AnnotationsType>(),
                "geometries"_a = RouteParameters::GeometriesType::Polyline,
                "overview"_a = RouteParameters::OverviewType::Simplified,
                "continue_straight"_a = std::optional<bool>(),
                "waypoints"_a = std::vector<std::size_t>(),
                "coordinates"_a = std::vector<osrm::util::Coordinate>(),
                "hints"_a = std::vector<std::optional<osrm::engine::Hint>>(),
                "radiuses"_a = std::vector<std::optional<double>>(),
                "bearings"_a = std::vector<std::optional<osrm::engine::Bearing>>(),
                "approaches"_a = std::vector<std::optional<osrm::engine::Approach>>(),
                "generate_hints"_a = true,
                "exclude"_a = std::vector<std::string>(),
                "snapping"_a = BaseParameters::SnappingType::Default,
                "Instantiates an instance of MatchParameters.\n"
                "Args:\n"
                "    - timestamps (list of unsigned int): Timestamps for the input locations in seconds since UNIX epoch. (default [])\n"
                "    - gaps (MatchParameters.GapsType): Allows the input track splitting based on huge timestamp gaps between points. (default split)\n"
                "    - tidy (bool): Allows the input track modification to obtain better matching quality for noisy tracks. (default False)\n"
                "    - RouteParameters (osrm.RouteParameters): Keyword arguments from parent class.\n"
                "Returns:\n"
                "    __init__ (osrm.MatchParameters): A MatchParameters object, for usage in Match.\n"
            )
        .def_rw("timestamps", &MatchParameters::timestamps)
        .def_rw("gaps", &MatchParameters::gaps)
        .def_rw("tidy", &MatchParameters::tidy)
        .def("IsValid", &MatchParameters::IsValid);
}