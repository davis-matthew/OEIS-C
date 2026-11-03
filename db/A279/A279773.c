/*
 * A279773  Numbers n such that the sum of digits of 3n equals 6.
 * https://oeis.org/A279773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A279773 – numbers n such that the sum of digits of 3n equals 6.
   The function enumerates numbers with digit sum 6 in increasing numeric
   order, divides each by 3 (the digitsum is 0 mod 3), and returns the
   n‑th value.  No external tables are used. */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper that generates all d‑digit numbers with digit sum 6
   in ascending order.  When the global counter 'cnt' reaches the target
   'n', the value 'cur/3' is stored into '*answer' and the function
   returns true to stop further recursion. */
static bool gen_len_rec(uint32_t pos, uint32_t rem_sum, seq_t cur,
                        uint32_t dlen, idx_t *cnt, idx_t n, seq_t *answer)
{
    /* If remaining sum cannot be achieved with the remaining places,
       prune the search. */
    if (rem_sum > 9u * (dlen - pos))
        return false;

    if (pos == dlen) {                /* full number constructed */
        if (rem_sum == 0) {
            (*cnt)++;                  /* one more qualifying n found   */
            if (*cnt == n) {            /* reached the requested n       */
                *answer = cur / (seq_t)3;
                return true;
            }
        }
        return false;
    }

    uint32_t start = (pos == 0) ? 1u : 0u;   /* no leading zeros       */
    for (uint32_t d = start; d <= 9u && rem_sum >= d; ++d) {
        seq_t next = cur * (seq_t)10 + (seq_t)d;
        if (gen_len_rec(pos + 1, rem_sum - d, next, dlen, cnt, n, answer))
            return true;       /* propagate success upwards */
    }
    return false;
}

/* Main function required by the build.  It returns the n‑th term of A279773. */
seq_t a(idx_t n)
{
    idx_t  cnt = 0;          /* number of terms found so far */
    seq_t  answer = 0;       /* result to be returned */

    /* Iterate over increasing digit lengths until the target term is found. */
    for (uint32_t dlen = 1; ; ++dlen) {
        if (gen_len_rec(0, 6u, (seq_t)0, dlen, &cnt, n, &answer))
            return answer;
    }
}
