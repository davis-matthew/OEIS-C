/*
 * A328287  Number of multiples of n which have only distinct and nonzero
 *          digits in base 10.
 * https://oeis.org/A328287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   dp_count, gen_numbers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A328287
 *  Number of multiples of n which have only distinct and nonzero digits in base 10.
 *  The algorithm:
 *  * For n % 10 == 0 or n > 987654321 the answer is 0.
 *  * For small n (≤ DP_THRESHOLD) we use a dynamic‑programming
 *    routine that enumerates all permutations of subsets of {1,…,9}
 *    while keeping track of the remainder modulo n.
 *  * For larger n we fall back to brute‑force: check every number
 *    with distinct non‑zero digits and count those divisible by n.
 *
 *  Complexity:
 *  • For n ≤ DP_THRESHOLD:   O(512 · n) time, O(512 · n) memory
 *  • For larger n:            O(986 409) time, O(1) memory
 *  Extent:  N = 987654321 is the largest number that can contain
 *  nine distinct non‑zero digits, therefore for n > N the answer
 *  is zero.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/dp_count.h"
#include "../utils/gen_numbers.h"

/* seq_t and idx_t are defined by the build system. */

#ifndef DP_THRESHOLD
#define DP_THRESHOLD 2000   /* threshold to switch to DP; adjust if needed */
#endif

/* Forward declarations for functions that build the list of numbers. */
static void build_numbers(void);
static void gen_numbers(int mask, seq_t val, seq_t *list, size_t *len, size_t *cap);

/* Global storage for all distinct‑non‑zero‑digit numbers. */
static seq_t *g_numbers = NULL;
static size_t g_num_len = 0;

/* Generate all numbers with distinct non‑zero digits (1–9). */
static void build_numbers(void)
{
    if (g_numbers != NULL) return;   /* already built */

    /* Rough upper bound for allocation: 900 000 is enough. */
    size_t cap = 1000000;
    g_numbers = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!g_numbers) return;          /* memory error */
    g_num_len = 0;

    /* Recursive generation. */
    gen_numbers(0, (seq_t)0, g_numbers, &g_num_len, &cap);
}

/* Brute‑force count for large n. */
static seq_t brute_count(idx_t n)
{
    build_numbers();
    seq_t cnt = 0;
    for (size_t i = 0; i < g_num_len; ++i) {
        /* n is non‑zero here */
        if (g_numbers[i] % (seq_t)n == 0) ++cnt;
    }
    return cnt;
}

/* ------------------------------------------------------------ */
/* Public function required by the problem statement            */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return 0;
    if (n % 10 == 0) return 0;
    if (n > (idx_t)987654321) return 0;

    if (n <= DP_THRESHOLD) {
        return dp_count(n);
    } else {
        return brute_count(n);
    }
}
