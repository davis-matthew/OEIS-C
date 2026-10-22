/*
 * A316432  Number of integer partitions of n whose length is equal to the GCD
 *          of all parts.
 * https://oeis.org/A316432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n whose length equals the
 *            GCD of all parts. Computation via partition DP and Möbius
 *            inversion
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: seq_t and idx_t are defined by the build system; do not redefine them. */
#include <stdbool.h>
#include <stddef.h>

/* Maximum n to pre‑compute.  The sequence is required only up to about 10 000,
   so a fixed upper bound is sufficient and keeps the program fast. */
#define MAX_N 10000

static seq_t a_arr[MAX_N + 1];   /* table of a(n) values */
static seq_t part_prev[MAX_N + 1];
static seq_t part_curr[MAX_N + 1];
static int    mu[MAX_N + 1];
static bool   is_comp[MAX_N + 1];
static idx_t  primes[MAX_N];
static size_t prime_cnt;

/* One‑time initialisation that pre‑computes μ, partitions, and a(n). */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* initialise tables */
    for (idx_t i = 0; i <= MAX_N; ++i) {
        a_arr[i]      = 0;
        part_prev[i]  = 0;
        part_curr[i]  = 0;
        mu[i]         = 0;
        is_comp[i]    = false;
    }
    part_prev[0] = 1;            /* p_0(0) = 1 */

    /* ----- compute Möbius values μ[1..MAX_N] ----- */
    mu[1] = 1;
    prime_cnt = 0;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            mu[i] = -1;           /* i is prime: μ(i) = -1 */
        }
        for (size_t j = 0; j < prime_cnt && i * primes[j] <= MAX_N; ++j) {
            idx_t v = i * primes[j];
            is_comp[v] = true;
            if (i % primes[j] == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    /* ----- compute all a(n) values ----- */
    seq_t *cur  = part_prev;   /* partitions for current L-1 */
    seq_t *next = part_curr;   /* partitions for current L */
    for (idx_t L = 1; L <= MAX_N; ++L) {
        idx_t Smax = MAX_N / L;

        /* compute partitions of numbers up to Smax into exactly L parts */
        next[0] = 0;
        for (idx_t S = 1; S <= Smax; ++S) {
            seq_t val = cur[S - 1];
            if (S >= L) val += next[S - L];
            next[S] = val;
        }

        /* add contributions for all n = L * S (1 ≤ S ≤ Smax) */
        for (idx_t S = 1; S <= Smax; ++S) {
            idx_t n = L * S;
            seq_t pos = 0, neg = 0;
            /* enumerate all divisors d of S */
            for (idx_t d = 1; d * d <= S; ++d) {
                if (S % d == 0) {
                    int mu_d = mu[d];
                    if (mu_d != 0) {
                        seq_t term = next[S / d];
                        if (mu_d > 0)
                            pos += term;
                        else
                            neg += term;
                    }
                    idx_t d2 = S / d;
                    if (d2 != d) {
                        int mu_d2 = mu[d2];
                        if (mu_d2 != 0) {
                            seq_t term = next[S / d2];
                            if (mu_d2 > 0)
                                pos += term;
                            else
                                neg += term;
                        }
                    }
                }
            }
            seq_t cnt = pos - neg;   /* difference is non‑negative */
            a_arr[n] += cnt;
        }

        /* swap buffers for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, a(0) is undefined */
    init_sequence();              /* ensure tables are ready */
    return a_arr[n];
}
