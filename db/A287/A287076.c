/*
 * A287076  a(n) = least k > n with the same sum of digits as n in some base b
 *          > 1.
 * https://oeis.org/A287076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > n with the same sum of digits as n in some base
 *            b > 1
 * Range:     u8 n <= 15, u16 n <= 1784, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9690 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute minimal k > n with same digit sum in base b. */
static seq_t f_base(seq_t n, idx_t b)
{
    /* Convert n to base b, store digits low to high. */
    /* Max number of digits for n <= 2^64 in base 2 is 65, safe with 128. */
    idx_t digits[128];
    idx_t len = 0;
    idx_t sum = 0;

    seq_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
        sum = 0;
    } else {
        while (tmp > 0) {
            // tmp % b and tmp / b
            seq_t q = tmp / (seq_t)b;
            seq_t r = tmp - q * (seq_t)b;
            digits[len++] = (idx_t)r;
            sum += (idx_t)r;
            tmp = q;
        }
    }

    bool found = false;
    seq_t best = 0;   /* placeholder */

    /* Case 1: modify existing digits. */
    for (idx_t j = 0; j < len; ++j) {
        if (digits[j] == b - 1) continue;  /* cannot increment this digit */

        /* Compute sum of higher digits (indices > j). */
        idx_t sum_high = 0;
        for (idx_t k = j + 1; k < len; ++k)
            sum_high += digits[k];

        idx_t rem = sum - sum_high - (digits[j] + 1);
        if (rem > (b - 1) * j) continue;    /* not enough room in lower positions */

        /* Build candidate digits. */
        idx_t cand[128];
        for (idx_t i = 0; i < len; ++i)
            cand[i] = digits[i];
        cand[j] = digits[j] + 1;
        for (idx_t i = 0; i < j; ++i)
            cand[i] = 0;   /* will fill minimal later */

        /* Distribute remaining sum into lower positions (least significant first). */
        idx_t residual = rem;
        for (idx_t i = 0; residual > 0 && i < j; ++i) {
            idx_t add = residual < (b - 1) ? residual : (b - 1);
            cand[i] = add;
            residual -= add;
        }

        /* Convert candidate digits back to value. */
        seq_t val = 0;
        seq_t power = 1;
        for (idx_t i = 0; i < len; ++i) {
            val += (seq_t)cand[i] * power;
            power *= (seq_t)b;
        }

        if (!found || val < best) {
            best = val;
            found = true;
        }
    }

    /* Case 2: add a new most significant digit (i.e., len). */
    /* New msb = 1; lower digits must sum to sum - 1. */
    idx_t need = sum - 1;
    if (need <= (b - 1) * len && need >= 0) {
        idx_t cand[128];
        for (idx_t i = 0; i < len; ++i)
            cand[i] = 0;
        cand[len] = 1;   /* new msb */

        idx_t residual = need;
        for (idx_t i = 0; residual > 0 && i < len; ++i) {
            idx_t add = residual < (b - 1) ? residual : (b - 1);
            cand[i] = add;
            residual -= add;
        }

        seq_t val = 0;
        seq_t power = 1;
        for (idx_t i = 0; i <= len; ++i) {
            val += (seq_t)cand[i] * power;
            power *= (seq_t)b;
        }

        if (!found || val < best) {
            best = val;
            found = true;
        }
    }

    /* best should always be found for b >= 2. */
    return best;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)2;
    }

    seq_t best = 0;
    bool found = false;

    for (idx_t b = 2; b <= n; ++b) {
        seq_t cand = f_base((seq_t)n, b);
        if (!found || cand < best) {
            best = cand;
            found = true;
        }
    }

    /* In the extremely unlikely event that no base b<=n yields a candidate
       (which cannot happen for n>=2), fall back to base 2. */
    if (!found)
        return f_base((seq_t)n, 2);

    return best;
}
