#ifndef datastructures_HEADER
#define datastructures_HEADER

#include "parser.hpp"

/*
 * Find the axis around which MirrorMap m can be reflected with
 * exactly REQUIRED_SMUDGES.
 *
 * Assume exactly one match exists.
 *
 * @param MirrorMap to scan
 * @return column around which it is reflectable or 100*row around which it is reflectable
 */
template <size_t REQUIRED_SMUDGES = 0>
size_t mirrorAxisVal(const MirrorMap& m){
    size_t nCols = m.first;
    size_t nRows = m.second.size() / nCols;

    // First check if it can be reflected around any horizontal axis
    // Due to row major storage of MirrorMap, this is slightly faster
    // due to more contiguous memory accesses.
    for(size_t i = 1; i < nRows; ++i){
        size_t smudges = 0;

        // need to compare row i at a negative and positive offset o
        for(size_t o = 0; i-o > 0 && i+1+o <= nRows; ++o){

            // compare both rows value by value
            // i.e., M[i-1+o, :] == M[i+o, :]
            // goto is used to continue outermost loop without additional compare
            for(size_t x = 0; x < nCols; ++x){
                if(m.second[(i-1-o) * nCols + x] != m.second[(i+o) * nCols + x])
                    if(++smudges > REQUIRED_SMUDGES)
                        goto early_exit_row_comparison;
            }
        }

        if(smudges == REQUIRED_SMUDGES)
            return i * 100;

        early_exit_row_comparison:;
    }

    // Same as above for M^t.
    // M^t[i-1+o, :] == M^t[i+o, :] <=> M[:, i-1+o] == M[:, i+o]
    for(size_t i = 1; i < nCols; ++i){
        // is it reflectable around col i?
        size_t smudges = 0;

        for(size_t o = 0; i-o > 0 && i+1+o <= nCols; ++o){
            for(size_t x = 0; x < nRows; ++x){
                if(m.second[x * nCols + (i-1-o)] != m.second[x * nCols + (i+o)])
                    if(++smudges > REQUIRED_SMUDGES)
                        goto early_exit_col_comparison;
            }
        }

        if(smudges == REQUIRED_SMUDGES)
            return i;

        early_exit_col_comparison:;
    }

    return 0;
}

#endif
