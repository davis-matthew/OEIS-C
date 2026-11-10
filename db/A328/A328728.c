/*
 * A328728  a(n) = Sum_{k = 0..w and t_k > 0} (-1)^t_k * 2^k, where Sum_{k =
 *          0..w} t_k * 3^k is the ternary representation of A328727(n).
 * https://oeis.org/A328728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A328749(A328727(n)); A328727(n) is the n-th number with
 *            no two adjacent nonzero base-3 digits (found by unranking), and
 *            A328749(m) = Sum_{k: t_k>0} (-1)^t_k * 2^k over the ternary
 *            digits t_k of m
 * Range:     i8 n <= 32, i16 n <= 1309, i32 n <= 5461, i64 n <= 5461,
 *            i128 n <= 5461, big any n
 * Validated: standard, pass, 76 terms through n = 5446 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define A328728_MAXLEN 40

static seq_t A328728_b[A328728_MAXLEN + 1];
static seq_t A328728_p3[A328728_MAXLEN + 1];
static bool A328728_inited = false;

static void A328728_init(void)
{
    if (A328728_inited)
        return;
    A328728_b[0] = 1;
    A328728_b[1] = 3;
    for (size_t i = 2; i <= A328728_MAXLEN; ++i)
        A328728_b[i] = A328728_b[i - 1] + (seq_t)2 * A328728_b[i - 2];
    A328728_p3[0] = 1;
    for (size_t i = 1; i <= A328728_MAXLEN; ++i)
        A328728_p3[i] = A328728_p3[i - 1] * 3;
    A328728_inited = true;
}

static seq_t A328728_suffix(idx_t rank, size_t len)
{
    seq_t value = 0;
    bool prev_nonzero = false;
    for (size_t pos = 0; pos < len; ++pos) {
        size_t remaining = len - pos;
        int digit;
        if (prev_nonzero) {
            digit = 0;
        } else {
            idx_t cnt0 = (idx_t)A328728_b[remaining - 1];
            if (rank < cnt0) {
                digit = 0;
            } else {
                rank -= cnt0;
                idx_t cnt1 = (remaining >= 2) ? (idx_t)A328728_b[remaining - 2] : 1;
                if (rank < cnt1) {
                    digit = 1;
                } else {
                    rank -= cnt1;
                    digit = 2;
                }
            }
        }
        value = value * 3 + digit;
        prev_nonzero = (digit != 0);
    }
    return value;
}

static seq_t A328728_A328727(idx_t n)
{
    A328728_init();
    if (n == 1)
        return 0;
    idx_t target = n - 2;
    size_t L = 1;
    idx_t cum = 0;
    while (true) {
        idx_t count;
        if (L == 1)
            count = 2;
        else
            count = (idx_t)((seq_t)2 * A328728_b[L - 2]);
        if (target < cum + count)
            break;
        cum += count;
        ++L;
    }
    idx_t rank = target - cum;
    if (L == 1)
        return (seq_t)(rank + 1);
    idx_t split = (idx_t)A328728_b[L - 2];
    seq_t msb = (rank < split) ? 1 : 2;
    if (rank >= split)
        rank -= split;
    seq_t suffix = A328728_suffix(rank, L - 2);
    return msb * A328728_p3[L - 1] + suffix;
}

seq_t a(idx_t n)
{
    seq_t v = A328728_A328727(n);
    seq_t m = v;
    seq_t res = 0;
    seq_t p = 1;
    while (m != 0) {
        int digit = (int)(m % 3);
        m /= 3;
        if (digit == 1)
            res -= p;
        else if (digit == 2)
            res += p;
        if (m != 0)
            p = p * 2;
    }
    return res;
}
