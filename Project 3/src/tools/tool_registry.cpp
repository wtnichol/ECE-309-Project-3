// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"
#include <cstdint>

ToolRegistry::ToolRegistry() : buckets(8) {} ;  // initializes the registry with 8 buckets for tools
ToolRegistry::~ToolRegistry() = default; // unique_ptrs in your buckets free the tools

std::size_t ToolRegistry::hash(std::string_view key) noexcept
{
    std::uint64_t h = 1469598103934665603ULL; // "offset basis"
    for (unsigned char c : key)        //given hash key and function
    { h ^= c; h *= 1099511628211ULL;} // "FNV prime"
    return static_cast<std::size_t>(h);
}

bool ToolRegistry::add(std::unique_ptr<Tool> tool)
{
    std::size_t slot; //stores which bucket the tool name should go in based on hash
    if (tool == nullptr) {  //checks if the tool actually exists
        return false;
    }
    std::string label = tool->name();  //stores the tool name
    if (find(label) != nullptr) { //checks if the tool name is registered in the bucket
        return false;
    }
    else {
        if (load_factor() >= 1.0) {   //if the bucket list is too full it will upsize the reg
            std::vector<std::vector<std::unique_ptr<Tool>>> newBuckets;  //creates a new temporary reg 
            newBuckets.resize(buckets.size() * 2); //doubles the reg for the newBuckets
            std::size_t newSlot; //acts as a temporary slot for the temp reg
            for (std::size_t i = 0; i < buckets.size(); i++) {  //nested for loops through the old reg
                for (std::size_t j = 0; j < buckets[i].size(); j++) {
                    newSlot = hash(buckets[i][j]->name()) % newBuckets.size(); //calculates the hash and slot for new expanded list
                    newBuckets[newSlot].push_back(std::move(buckets[i][j]));  //transfers ownership of these tools into the new bucket
                }
            }
            buckets = std::move(newBuckets); //transfers ownership of the reg back to buckets
        }
        slot = hash(tool->name()) % buckets.size(); //calculates hash slot of the new tool added
        buckets[slot].push_back(std::move(tool)); //transfers ownership of that tool to the slot to the reg
        size_++; //increases size of the registry
        return true;
    }
}

Tool *ToolRegistry::find(std::string_view name) const
{
    std::size_t slot;  
    slot = hash(name) % buckets.size(); //calculates hash of the tool i wanna locate
    for (std::size_t i = 0; i < buckets[slot].size(); i++) { //loops to find the tool
        if (buckets[slot][i]->name() == name) {
            return buckets[slot][i].get();  //if found return the pointer to that tool 
        }
    }
    return nullptr; //if not found return null
}

std::size_t ToolRegistry::size() const noexcept
{
    return size_; //return the size of the reg
}

void ToolRegistry::for_each(const std::function<void(const Tool &)> & fn) const
{
    for (std::size_t i = 0; i < buckets.size(); i++) {
        for (std::size_t j = 0; j < buckets[i].size(); j++) {  //loops through every slot and chain in the buckets
            fn(*buckets[i][j]); //calls the function of the tool it selects
        } 
    }
}

std::size_t ToolRegistry::bucket_count() const noexcept
{
    return buckets.size();  //returns number of buckets in the reg.
}

double ToolRegistry::load_factor() const noexcept
{
    double loadFactor;  
    loadFactor = (1.0 * size_) / buckets.size();  //calculates how full the reg is
    return loadFactor;
}

std::size_t ToolRegistry::max_probe_length() const
{
    std::size_t biggest = 0;
    for (std::size_t i = 0; i < buckets.size(); i++) { //loops through the reg
        if (buckets[i].size() >= biggest) { //if a slot/bucket contains a larger number of tools than the previous 
            biggest = buckets[i].size(); //records the largest tool number in a bucket
        }
    }
    return biggest; // return
}
