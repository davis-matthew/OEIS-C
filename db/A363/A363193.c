/*
 * A363193  a(1)=1, and thereafter a(n) = number of occurrences of a(k) among
 *          terms a(1..k), where k = n-a(n-1).
 * https://oeis.org/A363193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1: let k = n – a(n‑1), then a(n) = number of
 *            occurrences of a(k) among the terms a(1) … a(k). The
 *            implementation memoizes the sequence and its occurrence counts
 *            so that calls may be in any order
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* ---- C++ implementation ---- */
    static std::vector<seq_t> seq;   // 0‑based: seq[0] = a(1)
    static std::vector<seq_t> fcnt;  // fcnt[i] = number of occurrences of seq[i] up to index i
    static std::vector<seq_t> vals;  // distinct values seen
    static std::vector<seq_t> cnts;  // current count of each value in vals

    if (seq.size() < n) {
        seq.reserve(n);
        fcnt.reserve(n);
        if (vals.empty()) {          // initialise first term
            vals.push_back((seq_t)1);
            cnts.push_back((seq_t)1);
        }

        for (idx_t i = seq.size(); i < n; ++i) {
            if (i == 0) {             // a(1)
                seq.push_back((seq_t)1);
                fcnt.push_back((seq_t)1);
                continue;
            }

            idx_t prev = i - 1;
            idx_t k = (idx_t)(i + 1) - (idx_t)seq[prev];   // k = n - a(n-1), 1‑based
            // a(n) = fcnt[k-1]
            seq.push_back(fcnt[k - 1]);

            seq_t value = seq.back();

            /* find current count of value in the map */
            size_t pos = 0;
            bool found = false;
            while (pos < vals.size()) {
                if (vals[pos] == value) {
                    found = true;
                    break;
                }
                ++pos;
            }

            seq_t prev_cnt = found ? cnts[pos] : (seq_t)0;
            fcnt.push_back(prev_cnt + (seq_t)1);  // new occurrence count for this value

            if (found) {
                cnts[pos] = prev_cnt + (seq_t)1;
            } else {
                vals.push_back(value);
                cnts.push_back((seq_t)1);
            }
        }
    }
    return seq[n - 1];

#else
    /* ---- C implementation ---- */
    static seq_t *seq = NULL;        // a(1)…a(seq_len)
    static seq_t *fcnt = NULL;       // occurrence count up to index
    static seq_t *vals = NULL;       // distinct values seen
    static seq_t *cnts = NULL;       // current count of each value in vals
    static size_t len = 0;           // stored terms
    static size_t seq_cap = 0;       // capacity of seq, fcnt
    static size_t map_len = 0;       // number of distinct values
    static size_t map_cap = 0;       // capacity of vals, cnts

    /* grow sequence up to n terms */
    while (len < n) {
        /* ensure capacity for seq and fcnt */
        if (len >= seq_cap) {
            size_t newcap = seq_cap ? seq_cap * 2 : 4;
            seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
            fcnt = (seq_t *)realloc(fcnt, newcap * sizeof(seq_t));
            seq_cap = newcap;
        }

        if (len == 0) {              /* initialise a(1) */
            seq[0] = (seq_t)1;
            fcnt[0] = (seq_t)1;

            if (map_cap == 0) {       /* initialise map */
                map_cap = 4;
                vals = (seq_t *)malloc(map_cap * sizeof(seq_t));
                cnts = (seq_t *)malloc(map_cap * sizeof(seq_t));
            }
            vals[0] = (seq_t)1;
            cnts[0] = (seq_t)1;
            map_len = 1;
            ++len;
            continue;
        }

        /* compute next term a(n) */
        idx_t idx = (idx_t)len;                   // 0‑based index
        idx_t prev = idx - 1;                     // a(n‑1) at index prev
        idx_t k = idx + 1 - (idx_t)seq[prev];      // k = n – a(n‑1)
        seq[idx] = fcnt[k - 1];                    // a(n)
        seq_t value = seq[idx];

        /* find current count of this value */
        size_t pos = 0;
        bool found = false;
        while (pos < map_len) {
            if (vals[pos] == value) {
                found = true;
                break;
            }
            ++pos;
        }

        seq_t prev_cnt = found ? cnts[pos] : (seq_t)0;
        fcnt[idx] = prev_cnt + (seq_t)1;           // update occurrence count

        if (found) {
            cnts[pos] = prev_cnt + (seq_t)1;
        } else {
            /* grow map if necessary */
            if (map_len >= map_cap) {
                size_t newmcap = map_cap ? map_cap * 2 : 4;
                vals = (seq_t *)realloc(vals, newmcap * sizeof(seq_t));
                cnts = (seq_t *)realloc(cnts, newmcap * sizeof(seq_t));
                map_cap = newmcap;
            }
            vals[map_len] = value;
            cnts[map_len] = (seq_t)1;
            ++map_len;
        }

        ++len;
    }

    return seq[n - 1];
#endif
}
