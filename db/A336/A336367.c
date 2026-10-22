/*
 * A336367  Numbers k such that gcd(k, prime(k) + prime(k+1)) > 1.
 * https://oeis.org/A336367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that gcd(k, prime(k)+prime(k+1))
 *            > 1 Offsets: a(1) = 2, a(2) = 3, ... as in OEIS A336367. The
 *            function builds the sequence by scanning integers k and
 *            collecting those which satisfy the gcd condition. A static cache
 *            is used so any order of calls is supported efficien...
 * Range:     u8 n <= 19, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *terms      = NULL;
    static size_t terms_cap  = 0;
    static size_t terms_len  = 0;
    static idx_t last_k_checked = 1;   /* largest k already examined */

    if (n == 0) return (seq_t)0;   /* offset 1 – not used */

    /* Return cached result if available */
    if (terms_len >= (size_t)n)
        return terms[n - 1];

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* next candidate k */
        ++last_k_checked;

        uint32_t pk  = get_prime(last_k_checked);
        uint32_t pk1 = get_prime(last_k_checked + 1);

        seq_t k_seq = (seq_t)last_k_checked;
        seq_t sum   = (seq_t)pk + (seq_t)pk1;
        seq_t g     = gcd(k_seq, sum);

        if (g > (seq_t)1) {
            /* Ensure there is space in the cache */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
#ifdef __cplusplus
                seq_t *tmp = reinterpret_cast<seq_t*>(
#else
                seq_t *tmp = (seq_t*)(
#endif
                    realloc(terms, new_cap * sizeof(seq_t))
#ifdef __cplusplus
                                   );
#else
                                   );
#endif
                if (!tmp) abort();   /* allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k_seq;
        }
    }

    return terms[n - 1];
}
