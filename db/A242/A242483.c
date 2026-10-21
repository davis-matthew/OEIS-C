/*
 * A242483  Numbers n such that A242481(n) = ((n*(n+1)/2) mod n + sigma(n) mod
 *          n + antisigma(n) mod n) / n = 2.
 * https://oeis.org/A242483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of even n such that sigma(n) mod n > n/2. */

#include <stddef.h>
#include <stdint.h>

/* --- helper: sigma(n) modulo n --------------------------------- */
static idx_t sigma_mod(idx_t n)
{
    idx_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            sum += d;
            if (e != d)
                sum += e;
        }
    }
    return sum % n;
}

/* --- main sequence: a(idx_t n) -------------------------------- */
seq_t a(idx_t n)
{
    /* static storage for generated terms */
    static seq_t seq_arr[200000];
    static size_t seq_len = 0;
    static idx_t candidate = 1;          /* last integer examined */

    while (seq_len < n)
    {
        ++candidate;
        if (candidate & 1)      /* skip odd numbers */
            continue;

        idx_t mod = sigma_mod(candidate);
        if (mod > candidate / 2)
        {
            seq_arr[seq_len++] = (seq_t)candidate;
        }
    }

    return seq_arr[n - 1];
}
