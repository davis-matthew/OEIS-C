/*
 * A322370  For any n > 4: let p be the n-th prime number; a(n) is the least
 *          squarefree p-smooth integer congruent to 4 modulo p.
 * https://oeis.org/A322370
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 783, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute force search: least squarefree p‑smooth integer ≡ 4 (mod p) where p is the n‑th prime */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime generator ------------------------------------------------------- */
#define PRIME_LIMIT 200000  /* > prime(10000) = 104729 */

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list) return;          /* already initialised */

    /* allocate mark array */
    uint8_t *is_composite = (uint8_t *)malloc(PRIME_LIMIT + 1);
    if (!is_composite) return;       /* fatal error – unknown behaviour */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_composite[i] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) ++cnt;
    prime_count = cnt;

    /* store primes */
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    for (size_t i = 2, idx = 0; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i]) prime_list[idx++] = (uint32_t)i;

    free(is_composite);
}

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* undefined for indices below the offset */

    init_primes();                /* ensure prime list is ready */

    /* p = n-th prime (1‑based) */
    seq_t p = (seq_t)prime_list[n - 1];

    /* first value ≡ 4 (mod p) */
    seq_t candidate = (seq_t)4;

    for (;;)
    {
        seq_t t = candidate;
        bool ok = true;

        /* factor t by primes ≤ p (square‑free check) */
        for (size_t i = 0; i < prime_count; ++i)
        {
            uint32_t q = prime_list[i];

            /* avoid overflow conversion when seq_t is small */
            if (sizeof(seq_t) < 4 && q > 255) break;

            seq_t qseq = (seq_t)q;
            if (qseq > p) break;                     /* no larger primes needed */

            if (t % qseq == 0)
            {
                int exp = 0;
                while (t % qseq == 0)
                {
                    ++exp;
                    if (exp > 1) { ok = false; break; }
                    t /= qseq;
                }
                if (!ok) break;
            }
            if (t == (seq_t)1) break;
        }

        if (ok)
        {
            /* remaining factor >1 must be ≤ p (otherwise not p‑smooth) */
            if (t > (seq_t)1 && t > p) ok = false;
            if (t == (seq_t)1 && ok) return candidate;   /* found the least */
        }

        candidate += p;    /* next integer congruent to 4 (mod p) */
    }
}
