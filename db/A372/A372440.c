/*
 * A372440  Numbers k such that the least binary index of k plus the least
 *          prime index of k is even.
 * https://oeis.org/A372440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the nth integer k (n≥1) such that
    (least binary index of k) + (least prime index of k) is even.
    The least binary index is the position of the least‑significant 1
    bit (starting at 1).  The least prime index is the smallest m
    for which prime(m) divides k.  Both indices are considered modulo 2,
    so a number qualifies precisely when the two parities are equal.  */

#include <stdlib.h>
#include <stdint.h>

/* Assumes seq_t and idx_t are defined elsewhere in the build. */
seq_t a(idx_t n)
{
    /* ---------- static data --------------------------------------- */
    static uint32_t  *spf            = NULL;   /* smallest prime factor   */
    static unsigned char *prime_parity = NULL;   /* parity of prime index  */
    static size_t sieve_limit = 0;

    static seq_t  *cache     = NULL;   /* cached sequence values   */
    static size_t cache_len  = 0;      /* current number of cached terms   */
    static size_t cache_cap  = 0;      /* allocated capacity          */
    static idx_t  next_k      = 1;      /* next candidate integer to test */

    /* ---------- initialise sieve (once) -------------------------- */
    if (spf == NULL) {
        /* enough for n up to ~10^4; the sequence grows roughly linearly */
        sieve_limit = 2000000;               /* 2 000 000  < 2^31       */

        spf         = (uint32_t*)malloc((sieve_limit + 1) * sizeof(uint32_t));
        prime_parity = (unsigned char*)malloc((sieve_limit + 1) * sizeof(unsigned char));
        if (spf == NULL || prime_parity == NULL) exit(1);

        for (size_t i = 0; i <= sieve_limit; ++i) {
            spf[i]         = 0;
            prime_parity[i] = 0;
        }

        size_t prime_cnt = 0;   /* 1‑based index of primes */
        for (size_t i = 2; i <= sieve_limit; ++i) {
            if (spf[i] == 0) {                   /* i is prime            */
                spf[i] = (uint32_t)i;
                ++prime_cnt;
                prime_parity[i] = prime_cnt & 1;  /* parity of its index   */
                if (i * i <= sieve_limit) {
                    for (size_t j = i * i; j <= sieve_limit; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* ---------- initialise cache (once) --------------------------- */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
    }

    /* ---------- generate terms until we have n entries ------------- */
    while (cache_len < n) {
        ++next_k;
        if (next_k > (idx_t)sieve_limit) {
            /* Should never happen with the chosen sieve_limit. */
            exit(1);
        }
        if (next_k == 1) continue;           /* 1 has no prime divisor */

        /* Parity of least binary index: (tz+1)%2 */
        unsigned int tz = __builtin_ctz((unsigned) next_k);   /* trailing zeros */
        unsigned char lbi_parity = ((tz + 1) & 1);            /* 0 = even, 1 = odd */

        /* Parity of least prime index: parity stored for the smallest prime factor */
        uint32_t p = spf[(size_t)next_k];
        unsigned char pidx_parity = prime_parity[p];          /* 0 = even, 1 = odd */

        /* Accept if parities are equal (sum even) */
        if (lbi_parity == pidx_parity) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }

    /* ---------- return the requested term ------------------------- */
    return cache[n - 1];
}
