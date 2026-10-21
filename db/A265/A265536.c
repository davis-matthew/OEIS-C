/*
 * A265536  Largest base-6 palindrome m <= n, written in base 6.
 * https://oeis.org/A265536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   largest base‑6 palindrome m <= n, written in base 6
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to base‑6 digits, most significant first */
    unsigned char rev[20];
    int rev_len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        rev[0] = 0;
        rev_len = 1;
    } else {
        while (tmp > 0) {
            rev[rev_len++] = (unsigned char)(tmp % 6);
            tmp /= 6;
        }
    }
    unsigned char dig[20];
    int len = rev_len;
    for (int i = 0; i < len; ++i)
        dig[i] = rev[len - 1 - i];   /* most significant first */

    /* build palindrome candidate by mirroring */
    unsigned char pal[20];
    for (int i = 0; i < len; ++i)
        pal[i] = dig[i];
    for (int i = 0; i < len / 2; ++i)
        pal[len - 1 - i] = pal[i];

    /* compare cand <= original */
    int cmp = 0; /* 0: equal, -1: cand < orig, 1: cand > orig */
    for (int i = 0; i < len; ++i) {
        if (pal[i] < dig[i]) { cmp = -1; break; }
        if (pal[i] > dig[i]) { cmp = 1; break; }
    }

    if (cmp <= 0) { /* palindrome fits */
        seq_t res = 0;
        for (int i = 0; i < len; ++i)
            res = res * 10 + pal[i];
        return res;
    }

    /* candidate too large: decrement the left half (and middle if odd) */
    int k = len / 2;
    if (len % 2 == 0) { /* even length */
        int i = k - 1;
        while (i >= 0 && dig[i] == 0) {
            dig[i] = 5;
            --i;
        }
        if (i < 0) { /* underflow, produce all‑5 palindrome of length len-1 */
            seq_t res = 0;
            for (int j = 0; j < len - 1; ++j)
                res = res * 10 + 5;
            return res;
        }
        dig[i]--; /* decrement */
        if (dig[0] == 0) { /* leading zero appeared */
            seq_t res = 0;
            for (int j = 0; j < len - 1; ++j)
                res = res * 10 + 5;
            return res;
        }
    } else { /* odd length */
        int i = k; /* middle index */
        while (i >= 0 && dig[i] == 0) {
            dig[i] = 5;
            --i;
        }
        if (i < 0) { /* underflow */
            seq_t res = 0;
            for (int j = 0; j < len - 1; ++j)
                res = res * 10 + 5;
            return res;
        }
        dig[i]--; /* decrement middle or leftmost if middle was 0 */
        if (dig[0] == 0) { /* leading zero appeared */
            seq_t res = 0;
            for (int j = 0; j < len - 1; ++j)
                res = res * 10 + 5;
            return res;
        }
    }

    /* rebuild palindrome with new left half */
    for (int i = 0; i < len; ++i)
        pal[i] = dig[i];
    for (int i = 0; i < len / 2; ++i)
        pal[len - 1 - i] = pal[i];

    /* convert palindrome digits to decimal integer */
    seq_t res = 0;
    for (int i = 0; i < len; ++i)
        res = res * 10 + pal[i];
    return res;
}
