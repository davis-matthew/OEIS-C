/*
 * A241474  Smallest k such that tau(k)=reversal(k-n).
 * https://oeis.org/A241474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   divisor_count_u64, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest k>n such that tau(k) = rev(k-n)   (tau = divisor count, rev = decimal reverse) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Utility functions provided by the framework */
#include "../utils/rev_decimal.h"          /* seq_t rev_decimal(seq_t v); */
#include "../utils/divisor_count_u64.h"     /* uint64_t divisor_count_u64(uint64_t n); */

/* Local fast integer reversal (used for 64‑bit values) */
static seq_t rev_uint64(uint64_t x) {
    uint64_t r = 0;
    while (x) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return (seq_t)r;
}

seq_t a(idx_t n) {
    /* start searching from k = n+1, because k-n must be positive for rev() */
    uint64_t start = (uint64_t)n + 1;
    for (uint64_t k = start; ; ++k) {
        seq_t rev = rev_uint64(k - n);   /* rev(k-n) */
        uint64_t tau = divisor_count_u64(k);  /* tau(k) */
        if ((seq_t)tau == rev) {
            return (seq_t)k;             /* found the minimal k */
        }
    }
}
