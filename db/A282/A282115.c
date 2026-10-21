/*
 * A282115  Numbers m with k digits in base b (MSD(m)=d_k, LSD(m)=d_1) such
 *          that, for one of their digits in position d_k < j < d_1,
 *          Sum_{i=j+1..k} (i-j)*d_i = Sum_{i=1..j-1} (j-i)*d_i. Case b = 10.
 * https://oeis.org/A282115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3577, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, fulcrum_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282115: the nth integer that has a fulcrum digit j  
 * (1 < j < k) such that sum_{i=j+1..k} (i-j)*d_i == sum_{i=1..j-1} (j-i)*d_i  
 * in base 10, where the digits are d_k … d_1 (MSD … LSD).  
 * The function generates terms on demand, caches them, and returns the  
 * nth term (1‑based).  There is no closed‑form formula, so a search +   
 * memoisation approach is used. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/fulcrum_digit.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;   /* candidate to test next */

/* Return a(n) for the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but return 0 safely */

    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)last_checked;
        if (fulcrum_digit(cand))
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        ++last_checked;
    }
    return cache[(size_t)(n - 1)];
}
