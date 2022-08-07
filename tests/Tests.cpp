#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../Table.h"


// Cell tests
TEST_CASE("Cell")
{
    Cell cell(5,3,"IVAN");
    Cell empty;
    REQUIRE(cell.getColumn() == 3);
    REQUIRE(cell.getRow() == 5);
    REQUIRE(cell.getData() == "IVAN");
    SECTION("setData")
    {
        std::string name = "Petur";
        cell.setData(name);
        REQUIRE_FALSE(cell.getData()=="IVAN");
        REQUIRE(cell.getData()=="Petur");
    }
    REQUIRE(empty.getData()=="");
}

// Tree tests

TEST_CASE("Tree")
{
    Tree empty;
    SECTION("Empty tree")
    {
        REQUIRE(empty.getSize() == 0);
        REQUIRE(empty.getMaxRow() == 0);
        REQUIRE(empty.getMaxColumn() == 0);
    }
    SECTION("insert")
    {
        empty.insert(5,3,"530");
        REQUIRE(empty.getMaxRow() == 5);
        empty.insert(4,5,"abc");
        REQUIRE(empty.getMaxRow() == 5);
        REQUIRE(empty.getSize() == 2);
    }
    SECTION("count")
    {
        empty.insert(5,3,"530");
        empty.insert(4,5,"abc");
        REQUIRE(empty.count(0,0,2,2) == 0);
        REQUIRE(empty.count(0,0,4,5) == 1);
        REQUIRE(empty.count(0,0,5,5) == 2);
        REQUIRE_THROWS(empty.count(0,0,10,10));
    }
    SECTION("sum")
    {
        empty.insert(5,3,"530");
        empty.insert(4,5,"abc");
        REQUIRE(empty.sum(0,0,2,2) == 0);
        REQUIRE(empty.sum(0,0,4,5) == 0);
        REQUIRE(empty.sum(0,0,5,5) == 530);
        REQUIRE_THROWS(empty.sum(0,0,10,10));
        empty.insert(1,1,"=15*3+2"); //47
        empty.insert(0,0,"=R1C1+17"); // 64
        REQUIRE(empty.sum(0,0,1,1) == 47+64);
        empty.insert(1,1,"=15*3+ 2");
        REQUIRE_THROWS(empty.sum(0,0,1,1));
    }
    SECTION("getNodeExpr")
    {
        empty.insert(5,3,"530");
        empty.insert(4,5,"abc");
        empty.insert(1,1,"=5*3");
        REQUIRE(empty.getNodeExpr(5,3) == "530");
        REQUIRE(empty.getNodeExpr(1,1) == "=5*3");
        REQUIRE_THROWS(empty.getNodeExpr(4,3));
    }
    SECTION("getNodeValue")
    {
        empty.insert(5,3,"530");
        empty.insert(4,5,"abc");
        empty.insert(1,1,"=5*3");
        REQUIRE(empty.getNodeValue(5,3) == 530);
        REQUIRE(empty.getNodeValue(1,1) == 15);
        REQUIRE(empty.getNodeValue(4,5) == 0);
        REQUIRE_THROWS(empty.getNodeValue(4,3));
    }
    SECTION("change")
    {
        empty.insert(5,3,"530");
        REQUIRE_THROWS(empty.change(5,5,true));
    }
}