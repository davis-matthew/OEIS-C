/*
 * A272552  Numbers n such that n*prime(n) is a pandigital number containing
 *          digits 0-9 exactly once.
 * https://oeis.org/A272552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_counts, digit_len, get_prime, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: Returns the n‑th term (n ≥ 1) of OEIS A272552.
   A term is a number k such that k * prime(k) is a decimal 0‑9 pandigital
   (contains each digit 0‑9 exactly once). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Memoised array of found terms. */
    static seq_t *terms = NULL;      /* array of terms */
    static size_t terms_cnt = 0;     /* number of stored terms */
    static size_t terms_cap = 0;     /* capacity of the array */

    /* The next integer to test. */
    static idx_t next_k = 1;

    /* Find terms until we have at least n of them. */
    while (terms_cnt < n) {
        seq_t prod = (seq_t)next_k * (seq_t)get_prime(next_k);

        /* Only 10‑digit numbers can be 0‑9 pandigital. */
        if (digit_len(prod) == 10) {
            unsigned char cnt[10] = {0};

            digit_counts(prod, cnt);

            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] != 1) { ok = false; break; }
            }
            if (ok) {
                /* Ensure array has enough space and store the term. */
                grow_seq(&terms, &terms_cap, terms_cnt + 1);
                terms[terms_cnt++] = next_k;
            }
        }
        ++next_k;
    }

    return terms[n - 1];
}
