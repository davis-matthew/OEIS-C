/*
 * A288536  Eventual period of the RATS sequence in base n starting from 1; 0
 *          is for infinity.
 * https://oeis.org/A288536
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, no closed form. Iterate the RATS map x -> A288535(n,x)
 *            in base n, starting from 1. Each state is stored as its digit
 *            counts (the result is sorted, so the counts fully describe it).
 *            Brent's cycle detection returns the eventual period. a(n) = 0
 *            when the digit count grows past a large bound,...
 * Range:     u8 n <= 23, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 70 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A288536_MAXLEN   6000
#define A288536_MAXSTEPS 20000000L

/* one RATS step on a digit-count vector; returns 0 if length bound exceeded */
static int A288536_step(int *cnt, int b, int *d, int *s)
{
    int L = 0;
    for (int dg = 1; dg < b; dg++) L += cnt[dg];
    if (L > A288536_MAXLEN) return 0;
    int k = 0;
    for (int dg = 1; dg < b; dg++)
        for (int c = 0; c < cnt[dg]; c++) d[k++] = dg;   /* MSB first, ascending */
    int carry = 0;
    for (int i = L - 1; i >= 0; i--) {
        int v = d[i] + d[L - 1 - i] + carry;
        if (v >= b) { v -= b; carry = 1; } else carry = 0;
        s[i] = v;
    }
    memset(cnt, 0, sizeof(int) * (size_t)b);
    for (int i = 0; i < L; i++) cnt[s[i]]++;
    if (carry) cnt[1]++;
    cnt[0] = 0;   /* zeros become leading zeros after sorting */
    return 1;
}

seq_t a(idx_t n)
{
    int b = (int)n;
    if (b < 2) return 0;
    int *tort = (int *)malloc(sizeof(int) * (size_t)b);
    int *hare = (int *)malloc(sizeof(int) * (size_t)b);
    int *d = (int *)malloc(sizeof(int) * (size_t)(A288536_MAXLEN + 4));
    int *s = (int *)malloc(sizeof(int) * (size_t)(A288536_MAXLEN + 4));
    seq_t result = 0;
    if (!tort || !hare || !d || !s) {
        free(tort); free(hare); free(d); free(s);
        return 0;
    }
    memset(tort, 0, sizeof(int) * (size_t)b);
    tort[1] = 1;
    memcpy(hare, tort, sizeof(int) * (size_t)b);
    if (!A288536_step(hare, b, d, s)) goto done;

    {
        long power = 1, lam = 1, steps = 0;
        while (memcmp(tort, hare, sizeof(int) * (size_t)b) != 0) {
            if (power == lam) {
                memcpy(tort, hare, sizeof(int) * (size_t)b);
                power *= 2;
                lam = 0;
            }
            if (!A288536_step(hare, b, d, s)) goto done;
            lam++;
            if (++steps > A288536_MAXSTEPS) goto done;
        }
        result = (seq_t)lam;
    }
done:
    free(tort); free(hare); free(d); free(s);
    return result;
}
