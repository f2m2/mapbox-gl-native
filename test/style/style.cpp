#include <mbgl/test/util.hpp>
#include <mbgl/test/stub_file_source.hpp>

#include <mbgl/map/map_data.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/io.hpp>

using namespace mbgl;

TEST(Style, UnusedSource) {
    util::RunLoop loop;
    util::ThreadContext context { "Map", util::ThreadType::Map, util::ThreadPriority::Regular };
    util::ThreadContext::Set(&context);

    MapData data { MapMode::Still, GLContextMode::Unique, 1.0 };
    StubFileSource fileSource;
    Style style { data, fileSource };

    style.setJSON(util::read_file("test/fixtures/resources/style-unused-sources.json"), "");
    style.cascade();
    style.recalculate(0);

    Source *usedSource = style.getSource("usedsource");
    EXPECT_TRUE(usedSource);
    EXPECT_TRUE(usedSource->isLoaded());

    Source *unusedSource = style.getSource("unusedsource");
    EXPECT_TRUE(unusedSource);
    EXPECT_FALSE(unusedSource->isLoaded());
}

TEST(Style, UnusedSourceActiveViaClassUpdate) {
    util::RunLoop loop;
    util::ThreadContext context { "Map", util::ThreadType::Map, util::ThreadPriority::Regular };
    util::ThreadContext::Set(&context);

    MapData data { MapMode::Still, GLContextMode::Unique, 1.0 };
    StubFileSource fileSource;
    Style style { data, fileSource };

    data.addClass("visible");

    style.setJSON(util::read_file("test/fixtures/resources/style-unused-sources.json"), "");
    style.cascade();
    style.recalculate(0);

    Source *unusedSource = style.getSource("unusedsource");
    EXPECT_TRUE(unusedSource);
    EXPECT_TRUE(unusedSource->isLoaded());
}
