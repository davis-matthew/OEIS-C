/*
 * A279773  Numbers n such that the sum of digits of 3n equals 6.
 * https://oeis.org/A279773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gen_len_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A279773 – numbers n such that the sum of digits of 3n equals 6.
   The function enumerates numbers with digit sum 6 in increasing numeric
   order, divides each by 3 (the digitsum is 0 mod 3), and returns the
   n‑th value.  No external tables are used. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gen_len_rec.h"

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
