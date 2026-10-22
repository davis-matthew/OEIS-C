/*
 * A383093  Number of integer partitions of n that can be partitioned into
 *          constant blocks with a common sum.
 * https://oeis.org/A383093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S|n} sum_{g|(n/S)} mu(g) * * C( (n/S)/g + d(S) - 1 ,
 *            d(S) - 1 ) * where d(S) is the number of divisors of S and mu is
 *            the Möbius function. * This implements the counting of
 *            partitions of n that can be partitioned * into constant blocks
 *            with a common sum. * The implementation uses only...
 * Range:     u8 n <= 47, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef MAX_N
#define MAX_N 10000
#endif

/* ---------- Möbius function and divisor count precomputation ---------- */

static int  mu[MAX_N + 1];
static int  dcnt[MAX_N + 1];
static bool initialized = false;

static void precompute(void)
{
    int i, j;
    if (initialized) return;
    initialized = true;

    /* divisor count */
    for (i = 1; i <= MAX_N; ++i) {
        for (j = i; j <= MAX_N; j += i)
            dcnt[j]++;
    }

    /* Möbius function using linear sieve */
    int primes[2000], pcnt = 0;
    int is_comp[MAX_N + 1] = {0};
    mu[1] = 1;
    for (i = 2; i <= MAX_N; ++i) {
        if (!is_comp[i]) {
            primes[pcnt++] = i;
            mu[i] = -1;
        }
        for (j = 0; j < pcnt; ++j) {
            int p = primes[j];
            if (i * p > MAX_N) break;
            is_comp[i * p] = 1;
            if (i % p == 0) {
                mu[i * p] = 0;
                break;
            } else {
                mu[i * p] = -mu[i];
            }
        }
    }
}

/* ---------- binomial coefficient for seq_t ---------- */

static seq_t binom_seq_t(seq_t N, seq_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;
    seq_t res = (seq_t)1;
    for (size_t i = 1; i <= K; ++i) {
        res = res * (N - K + (seq_t)i) / (seq_t)i;
    }
    return res;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    precompute();

    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t total = (seq_t)0;
    idx_t i, div;

    /* iterate over all divisors of n */
    for (i = 1; (idx_t)i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor = i */
            div = i;
            {
                idx_t S = div;
                idx_t B = n / S;
                int D = dcnt[(int)S];
                seq_t pos = (seq_t)0, neg = (seq_t)0;

                /* contributions from all g dividing B */
                idx_t g;
                for (g = 1; (idx_t)g * g <= B; ++g) {
                    if (B % g == 0) {
                        int mu_g = mu[(int)g];
                        if (mu_g != 0) {
                            seq_t part = binom_seq_t(
                                (seq_t)(B / g) + (seq_t)D - 1,
                                (seq_t)D - 1
                            );
                            if (mu_g == 1) pos += part;
                            else if (mu_g == -1) neg += part;
                        }
                        idx_t g2 = B / g;
                        if (g2 != g) {
                            int mu_g2 = mu[(int)g2];
                            if (mu_g2 != 0) {
                                seq_t part = binom_seq_t(
                                    (seq_t)(B / g2) + (seq_t)D - 1,
                                    (seq_t)D - 1
                                );
                                if (mu_g2 == 1) pos += part;
                                else if (mu_g2 == -1) neg += part;
                            }
                        }
                    }
                }
                total += (pos - neg);
            }

            /* second divisor = n / i, avoid double counting when perfect square */
            if (i != n / i) {
                div = n / i;
                {
                    idx_t S = div;
                    idx_t B = n / S;
                    int D = dcnt[(int)S];
                    seq_t pos = (seq_t)0, neg = (seq_t)0;

                    idx_t g;
                    for (g = 1; (idx_t)g * g <= B; ++g) {
                        if (B % g == 0) {
                            int mu_g = mu[(int)g];
                            if (mu_g != 0) {
                                seq_t part = binom_seq_t(
                                    (seq_t)(B / g) + (seq_t)D - 1,
                                    (seq_t)D - 1
                                );
                                if (mu_g == 1) pos += part;
                                else if (mu_g == -1) neg += part;
                            }
                            idx_t g2 = B / g;
                            if (g2 != g) {
                                int mu_g2 = mu[(int)g2];
                                if (mu_g2 != 0) {
                                    seq_t part = binom_seq_t(
                                        (seq_t)(B / g2) + (seq_t)D - 1,
                                        (seq_t)D - 1
                                    );
                                    if (mu_g2 == 1) pos += part;
                                    else if (mu_g2 == -1) neg += part;
                                }
                            }
                        }
                    }
                    total += (pos - neg);
                }
            }
        }
    }
    return total;
}
