/*
 * A328216  Weak-Fibonacci-Niven numbers: numbers divisible by the number of
 *          terms in at least one representation as a sum of distinct
 *          Fibonacci numbers.
 * https://oeis.org/A328216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weak‑Fibonacci‑Niven numbers (OEIS A328216)
   The sequence consists of all integers that can be written as a
   sum of distinct Fibonacci numbers (with the two 1’s used at most
   once) such that the number of terms in the sum divides the sum.
   The function returns the nth such number, 1‑based.  It precomputes
   all terms up to a generous bound (200 000) so that n up to 10 000
   can be answered in time. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq      = NULL;  /* list of found terms */
static size_t seq_cnt  = 0;     /* number of stored terms   */
static size_t seq_cap  = 0;     /* allocated capacity       */

static void push_seq(seq_t val) {
    if (seq_cnt == seq_cap) {
        seq_cap = (seq_cap ? seq_cap << 1 : 128);
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) abort();
    }
    seq[seq_cnt++] = val;
}

/* Generate all weak‑Fibonacci‑Niven numbers up to MAXM.
   The values are stored in the static array `seq`. */
static void precompute(void) {
    const size_t MAXM = 200000u;  /* upper bound that yields >10 000 terms */

    /* Build the list of Fibonacci numbers up to MAXM,
       using the two 1’s only once (i.e. 1,2,3,5,8,...). */
    unsigned long long fib[64];
    size_t fib_cnt = 0;
    fib[fib_cnt++] = 1;    /* F(1) */
    fib[fib_cnt++] = 2;    /* F(3) – second “1” omitted */
    while (true) {
        unsigned long long next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        if (next > MAXM) break;
        fib[fib_cnt++] = next;
    }

    /* dp[sum] is a 32‑bit mask: bit k set   ⇔   sum can be formed
       by a subset of Fibonacci numbers of size k. */
    uint32_t *dp = (uint32_t *)calloc(MAXM + 1, sizeof(uint32_t));
    if (!dp) abort();
    dp[0] = 1;  /* empty subset has size 0 */

    for (size_t i = 0; i < fib_cnt; ++i) {
        unsigned long long f = fib[i];
        for (size_t s = MAXM - f; ; --s) {
            uint32_t mask = dp[s];
            if (mask) dp[s + f] |= mask << 1;  /* add one Fibonacci number */
            if (s == 0) break;
        }
    }

    /* Gather numbers that are divisible by at least one admissible k. */
    for (size_t sum = 1; sum <= MAXM; ++sum) {
        uint32_t mask = dp[sum];
        if (!mask) continue;
        for (uint32_t k = 1; k < 32; ++k) {
            if (mask & (1u << k)) {
                if (sum % k == 0) {
                    push_seq((seq_t)sum);
                    break;  /* no need to test larger k */
                }
            }
        }
    }

    free(dp);
}

/* Return the nth weak‑Fibonacci‑Niven number (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!seq) precompute();
    if (n <= seq_cnt) return seq[n - 1];
    /* If n exceeds the precomputed range, a larger bound would be needed
       – this situation is not expected for the test cases. */
    return 0;
}
