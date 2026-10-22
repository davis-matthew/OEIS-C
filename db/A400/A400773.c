/*
 * A400773  Numbers divisible by the square of their third smallest distinct
 *          prime factor.
 * https://oeis.org/A400773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1348, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400773: Numbers divisible by the square of their third smallest distinct prime factor.
   a(n) is the n‑th integer m such that m has at least three distinct prime factors and its
   third smallest distinct prime factor p3 satisfies p3^2 | m. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Spf array for numbers up to MAX_N */
#define MAX_N 2000000
static uint32_t *spf = NULL;           /* smallest prime factor for each number */
static size_t spf_size = 0;            /* current size of spf array */

/* initialise the spf array once */
static void init_spf(void)
{
    if (spf != NULL) return;
    spf = (uint32_t *)malloc((MAX_N + 1) * sizeof(uint32_t));
    if (spf == NULL) exit(EXIT_FAILURE); /* insufficient memory */

    for (size_t i = 0; i <= MAX_N; ++i) spf[i] = 0;

    for (size_t i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            if (i * i <= MAX_N) {
                for (size_t j = i * i; j <= MAX_N; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf_size = MAX_N;
}

/* test whether a number satisfies the condition of A400773 */
static bool satisfies(uint64_t x)
{
    if (x < 2) return false;

    uint64_t n = x;
    uint64_t prev = 0;
    int distinct = 0;
    uint64_t p3 = 0;
    int exp3 = 0;

    while (n > 1) {
        uint32_t p = spf[n];
        if (p != prev) {
            ++distinct;
            if (distinct == 3) {
                p3 = p;
                exp3 = 0;
            }
            prev = p;
        }
        if (distinct == 3 && p == p3) ++exp3;
        n /= p;
    }
    return (distinct >= 3) && (exp3 >= 2);
}

/* sequence cache */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_check = 2;

/* main sequence generator */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n = 0 */

    /* initialise helper tables once */
    init_spf();

    /* grow cache if needed */
    if (seq_arr == NULL) {
        seq_cap = 64;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_arr == NULL) exit(EXIT_FAILURE);
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (satisfies(next_check)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (seq_arr == NULL) exit(EXIT_FAILURE);
            }
            seq_arr[seq_len++] = (seq_t)next_check;
        }
        ++next_check;
        /* safety: if we exceed the precomputed spf range we cannot factor further */
        if (next_check > spf_size) {
            /* In practice this should never happen for n ≤ 10000 with MAX_N = 2 000 000 */
            break;
        }
    }

    return seq_arr[n - 1];
}
