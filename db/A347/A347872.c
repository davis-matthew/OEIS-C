/*
 * A347872  Numbers k such that k and A003415(sigma(k)) have the same parity.
 * https://oeis.org/A347872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  A347872: numbers k such that k and A003415(σ(k)) have the same parity
 *  (σ = sum of divisors)
 *  Returns the nth term (a(1) is the first term listed).
 *
 *  The implementation works by generating terms on demand and memoising
 *  them.  It precomputes a smallest‑prime‑factor (spf) table up to a
 *  fixed limit (greater than needed for the typical n < 10000).
 *
 *  Only standard C integer types and arithmetic are used; no
 *  floating‑point or non‑portable constructs are required.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Smallest prime factor table                                 */
/* ------------------------------------------------------------ */
#define SPF_LIMIT 200000 /* plenty for k up to ~10000 terms */

static uint32_t *spf = NULL;   /* array of uint32_t indices */
static bool spf_ready = false;

/* Build the spf sieve if it has not been built before. */
static void build_spf(void)
{
    if (spf_ready) return;

    spf = (uint32_t *)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= SPF_LIMIT; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            for (size_t j = i * i; j <= SPF_LIMIT; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_ready = true;
}

/* ------------------------------------------------------------ */
/*  Arithmetic helpers                                         */
/* ------------------------------------------------------------ */

/* Sum of divisors σ(k) via prime factorisation */
static uint64_t sigma_of(uint64_t n)
{
    if (n <= 1) return n;

    uint64_t result = 1;
    uint64_t temp = n;

    while (temp > 1) {
        uint64_t p;
        if (temp <= (uint64_t)SPF_LIMIT) {
            p = spf[temp];
        } else {                                 /* fallback trial division */
            p = temp;
            for (uint64_t d = 2; d * d <= temp; ++d) {
                if (temp % d == 0) { p = d; break; }
            }
        }

        uint64_t sum_pow = 1, power = 1;
        while (temp % p == 0) {
            temp /= p;
            power *= p;
            sum_pow += power;
        }
        result *= sum_pow;
    }
    return result;
}

/* Parity of A003415(n) according to the definition used in the OEIS entry.
 * The function returns 0 for even parity, 1 for odd parity.
 */
static int parity_a003415(uint64_t n)
{
    /* Count the exponent of 2 */
    unsigned v2 = 0;
    while ((n & 1ULL) == 0 && n > 0) {
        n >>= 1;
        ++v2;
    }

    if (v2 == 1) return 1;           /* odd parity */
    if (v2 == 0) {                   /* odd part only: parity of Ω(odd part) */
        int parity = 0;
        uint64_t temp = n;
        while (temp > 1) {
            uint64_t p;
            if (temp <= (uint64_t)SPF_LIMIT) {
                p = spf[temp];
            } else {
                p = temp;
                for (uint64_t d = 2; d * d <= temp; ++d) {
                    if (temp % d == 0) { p = d; break; }
                }
            }
            /* toggle parity for each occurrence of p */
            do {
                temp /= p;
                parity ^= 1;
            } while (temp % p == 0);
        }
        return parity;              /* parity of Ω(odd part) */
    }
    /* exponent of 2 > 1 : even parity */
    return 0;
}

/* ------------------------------------------------------------ */
/*  Memoised sequence storage                                  */
/* ------------------------------------------------------------ */

static seq_t *seq         = NULL;
static size_t seq_cap     = 0;
static size_t seq_len     = 0;   /* number of terms stored */
static uint64_t next_k     = 1;   /* next integer to test */

/* Ensure the seq array has space for 'need' elements (1‑based). */
static void seq_grow(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;

    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Public sequence accessor                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but safe */

    build_spf();

    while (seq_len < (size_t)n) {
        uint64_t k = next_k++;
        uint64_t sig = sigma_of(k);
        int parity_k = (int)(k & 1ULL);
        int parity_f = parity_a003415(sig);

        if (parity_k == parity_f) {
            seq_grow(seq_len + 1);
            seq[seq_len++] = (seq_t)k;
        }
    }

    return seq[n - 1];
}
