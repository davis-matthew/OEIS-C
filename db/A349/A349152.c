/*
 * A349152  Standard composition numbers of compositions into divisors.
 *          Numbers k such that all parts of the k-th composition in standard
 *          order are divisors of the sum of parts.
 * https://oeis.org/A349152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k (starting at 0) such that the
 *            composition of k in standard order has all parts dividing the
 *            sum of its parts
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether the k‑th composition (standard order) has all parts
   dividing its sum of parts. */
static bool valid_k(idx_t k)
{
    uint8_t parts[65] = {0};   /* at most 65 parts (bits of idx_t)      */
    size_t part_count = 0;     /* number of parts collected so far      */
    idx_t  bitpos = 1;         /* position of the current bit (start 1)*/
    idx_t  prev    = 0;        /* previous position of a 1              */
    idx_t  bits    = k;        /* local copy to shift                 */

    while (bits) {
        if (bits & 1ULL) {                    /* bit == 1? */
            parts[part_count++] = (uint8_t)(bitpos - prev); /* difference */
            prev = bitpos;
        }
        bitpos++;
        bits >>= 1;
    }

    /* Reverse the parts to obtain the standard order. */
    for (size_t i = 0; i < part_count / 2; ++i) {
        uint8_t tmp = parts[i];
        parts[i] = parts[part_count - 1 - i];
        parts[part_count - 1 - i] = tmp;
    }

    /* Compute the sum of parts. */
    seq_t sum = 0;
    for (size_t i = 0; i < part_count; ++i)
        sum += (seq_t)parts[i];

    /* Empty composition (k == 0) is considered valid. */
    if (sum == 0)
        return true;

    /* Check divisibility of each part by the sum. */
    for (size_t i = 0; i < part_count; ++i) {
        seq_t part = (seq_t)parts[i];
        if (sum % part != 0)
            return false;
    }
    return true;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* n is 1‑based: a(1)=0, a(2)=1, ... */
    idx_t k      = 0;      /* candidate integer          */
    idx_t count  = 0;      /* number of valid integers so far */

    while (true) {
        if (valid_k(k)) {
            count++;
            if (count == n)
                break;
        }
        k++;
    }
    return (seq_t)k;
}
