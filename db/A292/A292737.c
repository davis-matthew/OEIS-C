/*
 * A292737  Numbers in which 7 outnumbers all other digits together.
 * https://oeis.org/A292737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers in which 7 outnumbers all other digits together
 *            (unranking via combinatorial counting)
 * Range:     u8 n <= 3, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* binomial coefficient C(n,k), result stored in an idx_t (uint64_t). */
static idx_t binom_u64(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (unsigned i = 1; i <= k; ++i)
    {
        /* In each iteration res * (n - i + 1) is divisible by i. */
        res = res * (idx_t)(n - i + 1) / (idx_t)i;
    }
    return res;
}

/* 9^exp, result stored in an idx_t. */
static idx_t pow9(unsigned exp)
{
    idx_t r = 1;
    for (unsigned i = 0; i < exp; ++i)
        r = r * (idx_t)9;
    return r;
}

/* Number of ways to fill 'rem' remaining positions so that, together
   with cur7 occurrences of digit 7 in the already placed part,
   the resulting number of length 'tot' has more 7s than all other
   digits combined.  Returned as idx_t. */
static idx_t count_completions(unsigned rem, unsigned cur7, unsigned tot)
{
    unsigned needed_total = tot / 2 + 1;
    unsigned need_rem = 0;
    if (cur7 < needed_total)
        need_rem = needed_total - cur7;
    if (need_rem > rem) return 0;

    idx_t sum = 0;
    for (unsigned t = need_rem; t <= rem; ++t)
    {
        idx_t ways = binom_u64(rem, t);
        idx_t pow  = pow9(rem - t);
        sum += ways * pow;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the length of the n-th number. */
    unsigned d = 1;
    idx_t remaining = n;
    while (1)
    {
        idx_t count_len = 0;
        for (int first = 1; first <= 9; ++first)
        {
            unsigned init7 = (first == 7) ? 1 : 0;
            count_len += count_completions(d - 1, init7, d);
        }
        if (remaining <= count_len) break;
        remaining -= count_len;
        ++d;
    }

    /* Unrank the number of length d. */
    seq_t result = 0;
    unsigned cur7 = 0;
    for (unsigned pos = 0; pos < d; ++pos)
    {
        int start = (pos == 0) ? 1 : 0;
        for (int dig = start; dig <= 9; ++dig)
        {
            unsigned new7 = cur7 + (dig == 7 ? 1 : 0);
            unsigned rem = d - pos - 1;
            idx_t cnt   = count_completions(rem, new7, d);
            if (remaining <= cnt)
            {
                result = result * (seq_t)10 + (seq_t)dig;
                cur7 = new7;
                break;
            }
            remaining -= cnt;
        }
    }
    return result;
}
