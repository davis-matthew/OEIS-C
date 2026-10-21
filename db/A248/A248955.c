/*
 * A248955  Number of polynomials a_k*x^k + ... + a_1*x + n with k > 0,
 *          integer coefficients and distinct positive integer roots and
 *          positive integers n.
 * https://oeis.org/A248955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑empty subsets of D_n whose product divides
 *            n
 * Range:     u8 n <= 359, u16 n <= 2159, u32 n <= 2159, u64 n <= 2159,
 *            u128 n <= 2159, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses dynamic programming over the exponent vectors of n’s divisors. */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the recursive divisor generation helper. */
static void gen_divisors(int r, const int *exp_arr, int *cur_exp, int pos,
                         seq_t *dp, size_t states,
                         const unsigned char *idx_exp,
                         const size_t *stride);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Factorise n.  We only need the prime exponents. */
    uint64_t m = (uint64_t)n;
    uint64_t primes[20];
    int exps[20];
    int r = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[r] = p;
            exps[r] = e;
            ++r;
        }
    }
    if (m > 1) {  /* Remaining prime factor */
        primes[r] = m;
        exps[r] = 1;
        ++r;
    }

    /* Compute number of states: product of (exp+1) for each prime. */
    size_t states = 1;
    for (int i = 0; i < r; ++i) states *= (size_t)(exps[i] + 1);

    /* Strides for linear index mapping. */
    size_t *stride = NULL;
    if (r > 0) {
        stride = (size_t*)malloc(r * sizeof(size_t));
        stride[0] = 1;
        for (int i = 1; i < r; ++i)
            stride[i] = stride[i - 1] * (size_t)(exps[i - 1] + 1);
    }

    /* Precompute exponent vector for every state index. */
    unsigned char *idx_exp = NULL;
    if (states * (size_t)r > 0) {
        idx_exp = (unsigned char*)malloc(states * (size_t)r);
        for (size_t idx = 0; idx < states; ++idx) {
            size_t tmp = idx;
            for (int j = 0; j < r; ++j) {
                idx_exp[idx * (size_t)r + j] = (unsigned char)(tmp % (exps[j] + 1));
                tmp /= (size_t)(exps[j] + 1);
            }
        }
    }

    /* DP array: dp[sum_vector] = number of subsets achieving that sum. */
    seq_t *dp = (seq_t*)calloc(states, sizeof(seq_t));
    dp[0] = 1;  /* Empty subset */

    /* Recursively generate all divisors and update DP. */
    int cur_exp[20] = {0};
    gen_divisors(r, exps, cur_exp, 0, dp, states, idx_exp, stride);

    /* Sum all subsets and subtract the empty subset. */
    seq_t total = 0;
    for (size_t i = 0; i < states; ++i) total += dp[i];
    seq_t result = total - (seq_t)1;  /* Exclude empty subset */

    /* Free allocated memory. */
    free(dp);
    free(idx_exp);
    free(stride);

    return result;
}

/* Recursive helper to generate all divisors via their exponent vectors. */
static void gen_divisors(int r, const int *exp_arr, int *cur_exp, int pos,
                         seq_t *dp, size_t states,
                         const unsigned char *idx_exp,
                         const size_t *stride)
{
    if (pos == r) {
        /* Compute linear offset for this divisor's exponent vector. */
        size_t posV = 0;
        for (int j = 0; j < r; ++j) posV += (size_t)cur_exp[j] * stride[j];

        /* Update DP in reverse to avoid reuse of the same divisor. */
        for (size_t i = states; i-- > 0; ) {
            bool ok = true;
            for (int j = 0; j < r; ++j) {
                if ((unsigned char)idx_exp[i * (size_t)r + j] + cur_exp[j] > exp_arr[j]) {
                    ok = false; break;
                }
            }
            if (ok) dp[i + posV] += dp[i];
        }
        return;
    }

    for (int e = 0; e <= exp_arr[pos]; ++e) {
        cur_exp[pos] = e;
        gen_divisors(r, exp_arr, cur_exp, pos + 1,
                     dp, states, idx_exp, stride);
    }
}
