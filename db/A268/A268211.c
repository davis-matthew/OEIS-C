/*
 * A268211  Numbers n of the form 2^k + 1 such that n + k is a prime q (for k
 *          >= 0).
 * https://oeis.org/A268211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers of the form 2^k + 1 with 2^k + 1 + k prime. The
 *            program generates terms by brute‑forcing successive k and
 *            testing primality of 2^k + 1 + k
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow_seq.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* Returns the n-th term of sequence A268211 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* index 0 is outside the offset */

    /* Static state to allow non‑sequential calls */
    static seq_t *seq  = NULL;
    static size_t len  = 0;
    static size_t cap  = 0;
    static idx_t  next_k = 0;      /* next exponent to test */

    /* Generate terms up to n */
    while (len < n) {
        seq_t power = pow_seq((seq_t)2, (int)next_k);   /* 2^k */
        seq_t val   = power + (seq_t)1;                 /* 2^k + 1 */
        seq_t test  = val + (seq_t)next_k;              /* 2^k + 1 + k */

        if (is_prime_big(test)) {  /* prime test */
            append_candidate(&seq, &len, &cap, val);
        }
        next_k++;
    }

    return seq[n-1];
}
