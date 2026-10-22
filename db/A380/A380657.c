/*
 * A380657  Numbers whose prime factorization has more Pythagorean prime
 *          factors than non-Pythagorean prime factors (including
 *          multiplicities).
 * https://oeis.org/A380657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose prime factorization contains more Pythagorean
 * prime factors (primes ≡ 1 mod 4) than non‑Pythagorean prime factors.
 * A prime 2 is treated as non‑Pythagorean here, matching the OEIS data.
 */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n = 0 */

    /* static cache of terms already found */
    static seq_t *cache = NULL;
    static size_t size  = 0;             /* number of stored terms   */
    static size_t cap   = 0;             /* allocated capacity       */
    static uint64_t checked = 1;         /* largest candidate examined */

    /* grow cache until it contains at least n terms */
    while (size < n) {
        /* examine next candidate integer */
        uint64_t cand = ++checked;
        uint64_t num = cand;
        int py = 0, non = 0;

        /* factor 2: it is a non‑Pythagorean prime */
        if ((num & 1) == 0) {
            int e = 0;
            do { num >>= 1; ++e; } while ((num & 1) == 0);
            non += e;
        }

        /* trial division by odd numbers up to sqrt(num) */
        for (uint64_t p = 3; p <= num / p; p += 2) {
            if (num % p != 0) continue;
            int e = 0;
            while (num % p == 0) { num /= p; ++e; }
            if ((p & 3) == 1) py += e;      /* p ≡ 1 (mod 4) */
            else              non += e;     /* p ≡ 3 (mod 4) */
        }

        /* remaining prime factor > sqrt */
        if (num > 1) {
            if ((num & 3) == 1) ++py;
            else                 ++non;
        }

        /* include candidate if Pythagorean count exceeds non‑Pythagorean */
        if (py > non) {
            if (size == cap) {           /* grow the array */
                cap = cap ? cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; size = cap = 0;
                              return 0; } /* memory failure */
                cache = tmp;
            }
            cache[size++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
