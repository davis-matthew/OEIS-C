/*
 * A306864  Lexicographically earliest sequence of distinct positive terms
 *          such that among the prime divisors of the product of two
 *          consecutive terms there are at least 4 runs of consecutive prime
 *          numbers.
 * https://oeis.org/A306864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   A306864(a(n)) is defined by a search: * a(1) = 1 * for n > 1,
 *            a(n) is the smallest unused positive integer x * such that among
 *            the prime divisors of a(n-1)*x there are at least * four runs of
 *            consecutive prime numbers. The sequence is a * permutation of
 *            the natural numbers
 * Range:     u8 n <= 1, u16 n <= 9959, u32 n <= 9959, u64 n <= 9959,
 *            u128 n <= 9959, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

#define BITSET_WORDS(n)   (((n) + 63) >> 6)

/*--- Static storage for the sequence and used numbers ----------------*/
static seq_t *g_seq      = NULL;   /* array of generated terms   */
static size_t g_seq_cap  = 0;      /* capacity of g_seq          */
static size_t g_seq_len  = 0;      /* current number of terms    */

static uint64_t *g_used   = NULL;  /* bitset of used numbers     */
static size_t    g_used_cap = 0;    /* words of g_used           */

/*--- Bitset helpers --------------------------------------------------*/
static void ensure_used(uint64_t max_val)
{
    size_t needed = BITSET_WORDS(max_val + 1);
    if (needed > g_used_cap) {
        uint64_t *tmp = (uint64_t *)realloc(g_used,
                                            needed * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        for (size_t i = g_used_cap; i < needed; ++i)
            tmp[i] = 0;
        g_used   = tmp;
        g_used_cap = needed;
    }
}

static bool used_flag(uint64_t v)
{
    if (v >= g_used_cap << 6) return false;
    size_t w = v >> 6;
    size_t b = v & 63;
    return (g_used[w] >> b) & 1;
}

static void set_used(uint64_t v)
{
    ensure_used(v);
    size_t w = v >> 6;
    size_t b = v & 63;
    g_used[w] |= ((uint64_t)1u << b);
}

/*--- Check whether the product of two numbers has at least four runs --
 *    of consecutive primes.  The arguments are 64‑bit integers. --------*/
static bool have_four_runs(uint64_t p1, uint64_t p2)
{
    uint64_t primes1[64], exps1[64];
    size_t cnt1 = 0;
    factor_u64(p1, primes1, exps1, &cnt1);

    uint64_t primes2[64], exps2[64];
    size_t cnt2 = 0;
    factor_u64(p2, primes2, exps2, &cnt2);

    uint64_t merged[128];
    size_t i = 0, j = 0, m = 0;
    while (i < cnt1 && j < cnt2) {
        if (primes1[i] == primes2[j]) { merged[m++] = primes1[i]; ++i; ++j; }
        else if (primes1[i] < primes2[j]) { merged[m++] = primes1[i]; ++i; }
        else { merged[m++] = primes2[j]; ++j; }
    }
    while (i < cnt1) merged[m++] = primes1[i++];
    while (j < cnt2) merged[m++] = primes2[j++];

    if (m == 0) return false;

    int runs = 1;
    for (size_t k = 1; k < m; ++k) {
        if (next_prime(merged[k-1]) != merged[k]) {
            if (++runs >= 4) return true;
        }
    }
    return runs >= 4;
}

/*--- Public entry point ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0 */

    /* initialise data structures on first call */
    if (g_seq_len == 0) {
        g_seq_cap = 64;
        g_seq     = (seq_t *)malloc(g_seq_cap * sizeof(seq_t));
        g_used_cap = 0;
        g_used = NULL;
        /* first term */
        g_seq[0] = (seq_t)1;
        g_seq_len = 1;
        set_used(1);
    }

    /* grow the sequence until we have at least n terms */
    while (g_seq_len < (size_t)n) {
        uint64_t prev = (uint64_t)g_seq[g_seq_len - 1];
        uint64_t cand = 1;

        for (;;) {
            if (!used_flag(cand) && have_four_runs(prev, cand)) {
                /* accept this candidate */
                if (g_seq_len >= g_seq_cap) {
                    g_seq_cap = (g_seq_cap < 64) ? 64 : g_seq_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(g_seq,
                                                  g_seq_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    g_seq = tmp;
                }
                g_seq[g_seq_len++] = (seq_t)cand;
                set_used(cand);
                break;
            }
            ++cand;
        }
    }

    return g_seq[n - 1];
}
