/*
 * A333921  a(n) is the least k such that the decimal representation of n
 *          appears as a substring in the concatenation of 0, 1, ..., k.
 * https://oeis.org/A333921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that the decimal representation of n appears
 *            as a substring of the concatenation of 0,1,…,k
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build target string t from integer n */
    char tbuf[32];
    int tlen = 0;
    if (n == 0) {
        tbuf[0] = '0';
        tlen = 1;
    } else {
        idx_t tmp = n;
        char rev[20];
        int dlen = 0;
        while (tmp > 0) {
            rev[dlen++] = '0' + (tmp % 10);
            tmp /= 10;
        }
        for (int i = dlen - 1; i >= 0; --i) {
            tbuf[tlen++] = rev[i];
        }
    }

    /* Compute prefix-function for KMP */
    int pi[32];
    pi[0] = 0;
    for (int i = 1; i < tlen; ++i) {
        int k = pi[i - 1];
        while (k > 0 && tbuf[k] != tbuf[i]) k = pi[k - 1];
        if (tbuf[k] == tbuf[i]) ++k;
        pi[i] = k;
    }

    /* Scan the stream of digits from 0 up to n */
    int state = 0;   /* how many characters of t are currently matched */

    for (idx_t j = 0; j <= n; ++j) {
        /* Process the decimal digits of j */
        if (j == 0) {
            char d = '0';
            while (state > 0 && tbuf[state] != d) state = pi[state - 1];
            if (tbuf[state] == d) ++state;
            if (state == tlen) return (seq_t)j;
        } else {
            idx_t tmp = j;
            char rev[20];
            int dlen = 0;
            while (tmp > 0) {
                rev[dlen++] = '0' + (tmp % 10);
                tmp /= 10;
            }
            for (int i = dlen - 1; i >= 0; --i) {
                char d = rev[i];
                while (state > 0 && tbuf[state] != d) state = pi[state - 1];
                if (tbuf[state] == d) ++state;
                if (state == tlen) return (seq_t)j;
            }
        }
    }

    /* Should never reach here because n itself always yields a match */
    return (seq_t)n;
}
