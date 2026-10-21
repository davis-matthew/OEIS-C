/*
 * A291438  Smallest prime of the form (2*n)*3^m + 1 for some m >= 0, or -1 if
 *          no such prime exists.
 * https://oeis.org/A291438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 27, u32 n <= 27, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A291438 – smallest prime of the form (2*n)*3^m + 1
   (m ≥ 0), or –1 if no such prime exists.  Computed by a simple
   search over increasing powers of 3. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n==0 is undefined */
    if (n == 0) {
        /* Spec: no defined value, but we return –1 to avoid UB */
        return (seq_t)(-1);
    }

    /* 2*n  */
    seq_t base = (seq_t)n * (seq_t)2;

    /* current power of 3 (pow3 = 3^m), start with 3^0 = 1 */
    seq_t pow3 = (seq_t)1;

    /* For small integer types we guard against overflow. */
    const bool small = (sizeof(seq_t) <= 8);
    unsigned __int128 max_seq = 0;
    if (small) {
        unsigned int bits = (unsigned int)(sizeof(seq_t) * 8);
        /* bits ≤ 64 so shift is well defined */
        max_seq = (((unsigned __int128)1) << bits) - 1;
    }

    /* Simple search over m = 0,1,2,… */
    while (true) {
        /* product = base * pow3  (may overflow for small types) */
        bool overflow = false;
        unsigned __int128 prod = 0;
        if (small) {
            prod = (unsigned __int128)base * (unsigned __int128)pow3;
            if (prod > max_seq) overflow = true;          /* candidate would overflow seq_t */
        } else {
            /* seq_t is arbitrary‑precision; multiplication cannot overflow */
            prod = (unsigned __int128)0; /* unused */
        }

        if (!overflow) {
            seq_t candidate;
            if (small) {
                candidate = (seq_t)(prod + (unsigned __int128)1);
            } else {
                candidate = base * pow3 + (seq_t)1;
            }

            if (is_prime_big(candidate))
                return candidate;
        }

        /* Prepare next power of 3.  Stop if it would overflow seq_t. */
        if (small) {
            unsigned __int128 next_pow3 = (unsigned __int128)pow3 * 3;
            if (next_pow3 > max_seq / (unsigned __int128)base)
                break;                     /* further terms would overflow */
            pow3 = (seq_t)next_pow3;
        } else {
            pow3 = pow3 * (seq_t)3;        /* unbounded */
        }
    }

    /* No prime found before overflow (or infinite search aborted). */
    return (seq_t)(-1);
}
