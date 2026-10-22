/*
 * A309870  a(n) is the smallest number whose digits are 1's and 0's that
 *          cannot be written as a concatenation of any of the previous terms
 *          (not repeating any terms in the concatenation). a(0) = 0.
 * https://oeis.org/A309870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 108, u64 n <= 499, u128 n <= 499,
 *            big any n
 * Helpers:   candidate_from_len_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309870: smallest decimal number with digits 0/1 that
   cannot be written as a concatenation of distinct previously
   generated terms.  Computed by incremental search. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/candidate_from_len_mask.h"

/* ---------------------------------------------------------------------------
   Internal data structures.  Each generated term is stored with its
   numeric value and its decimal string representation.  The dynamic
   arrays are expanded as needed.  All allocations use explicit casts
   so that the code compiles as C++.
   --------------------------------------------------------------------------- */
typedef struct {
    seq_t  val;   /* numeric value of the term */
    char  *str;   /* decimal string, null‑terminated */
    size_t len;   /* length of the string (excluding NUL) */
} term_t;

/* static state:  terms, their count, and array capacity */
static term_t *terms      = NULL;
static size_t  term_cnt   = 0;
static size_t  term_cap   = 0;

/* auxiliary array used during the DFS search; its size is reset to
   term_cnt on each candidate test. */
static bool   *used       = NULL;
static size_t  used_cap   = 0;

/* generators for the next candidate with only digits 0/1 */
static idx_t   cur_len       = 1;              /* current decimal length */
static uint64_t cur_mask      = 0;              /* mask of digits after the
                                                   leading 1 (0/1 bits) */
static bool    initialized   = false;          /* a(0)=0 inserted? */

/* ---------------------------------------------------------------------------
   Resizing helpers.
   --------------------------------------------------------------------------- */
static void ensure_terms_cap(size_t need)
{
    if (need <= term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    term_t *new_arr = (term_t *)realloc(terms, new_cap * sizeof(term_t));
    if (!new_arr) abort();
    terms = new_arr;
    term_cap = new_cap;
}

static void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    bool *new_arr = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_arr) abort();
    used = new_arr;
    used_cap = new_cap;
}

/* ---------------------------------------------------------------------------
   Depth‑first search to decide whether a candidate can be partitioned
   into distinct earlier terms.
   --------------------------------------------------------------------------- */
static bool dfs(const char *cand, size_t cand_len, size_t pos)
{
    if (pos == cand_len) return true;   /* all characters consumed */

    for (size_t i = 0; i < term_cnt; ++i) {
        if (!used[i]) {
            size_t tlen = terms[i].len;
            if (pos + tlen > cand_len) continue;
            if (cand[pos] != terms[i].str[0]) continue;
            if (!memcmp(cand + pos, terms[i].str, tlen)) {
                used[i] = true;
                if (dfs(cand, cand_len, pos + tlen)) return true;
                used[i] = false;
            }
        }
    }
    return false;
}

static bool can_be_represented(const char *cand)
{
    size_t len = strlen(cand);
    ensure_used_cap(term_cnt);
    for (size_t i = 0; i < term_cnt; ++i) used[i] = false;
    return dfs(cand, len, 0);
}

/* ---------------------------------------------------------------------------
   Public function as required by the build system.
   --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise a(0) on first call */
    if (!initialized) {
        ensure_terms_cap(1);
        terms[0].val = (seq_t)0;
        terms[0].len = 1;
        terms[0].str = (char *)malloc(2);
        if (!terms[0].str) abort();
        terms[0].str[0] = '0';
        terms[0].str[1] = '\0';
        term_cnt = 1;
        cur_len = 1;
        cur_mask = 0;
        initialized = true;
    }

    /* generate terms until we have at least n+1 of them */
    while (term_cnt <= n) {
        char cand_str[64];
        seq_t cand_val = candidate_from_len_mask(cur_len, cur_mask, cand_str);

        if (!can_be_represented(cand_str)) {
            ensure_terms_cap(term_cnt + 1);
            terms[term_cnt].val = cand_val;
            terms[term_cnt].len = strlen(cand_str);
            terms[term_cnt].str = (char *)malloc(terms[term_cnt].len + 1);
            if (!terms[term_cnt].str) abort();
            memcpy(terms[term_cnt].str, cand_str, terms[term_cnt].len + 1);
            term_cnt++;
        }

        /* advance to next candidate of the same length; when all masks
           are exhausted increase the length */
        if (++cur_mask < ((uint64_t)1 << (cur_len - 1)))
            continue;
        else {
            cur_len++;
            cur_mask = 0;
        }
    }
    return terms[n].val;
}
