/*
 * A262223  a(n) = n + largest palindrome contained in decimal representation
 *          of n.
 * https://oeis.org/A262223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = n + largest palindrome contained in decimal representation of n */
seq_t a(idx_t n)
{
    /* Extract decimal digits of n into an array (max 20 digits for 64‑bit). */
    int digits[20];
    int len = 0;
    if (n == 0)
    {
        digits[0] = 0;
        len = 1;
    }
    else
    {
        idx_t t = n;
        while (t > 0)
        {
            digits[len++] = t % 10;
            t /= 10;
        }
        /* Reverse to correct order. */
        for (int i = 0; i < len / 2; ++i)
        {
            int tmp = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = tmp;
        }
    }

    seq_t max_pal = 0;

    /* Enumerate all palindromic substrings by expanding around each center. */
    for (int center = 0; center < len; ++center)
    {
        /* Odd length palindromes. */
        int l = center, r = center;
        while (l >= 0 && r < len && digits[l] == digits[r])
        {
            seq_t val = 0;
            for (int i = l; i <= r; ++i)
                val = val * 10 + digits[i];
            if (val > max_pal)
                max_pal = val;
            --l;
            ++r;
        }

        /* Even length palindromes. */
        l = center;
        r = center + 1;
        while (l >= 0 && r < len && digits[l] == digits[r])
        {
            seq_t val = 0;
            for (int i = l; i <= r; ++i)
                val = val * 10 + digits[i];
            if (val > max_pal)
                max_pal = val;
            --l;
            ++r;
        }
    }

    return (seq_t)n + max_pal;
}
