/*
 * A359020  Number of inequivalent tilings of a 4 X n rectangle using
 *          integer-sided square tiles.
 * https://oeis.org/A359020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 4, with A = A054856 (a(0..4) = 1,1,5,13,40; A(n) =
 *            2A(n-1) + 3A(n-2) - A(n-4) - A(n-5)) and compo(n) = compo(n-1) +
 *            2*compo(n-2) + compo(n-4), compo(0) = compo(1) = 1, compo(2) =
 *            3, compo(3) = 5: even n: a(n) = (A(n) + compo(n) + 4A((n-2)/2) +
 *            2A((n-4)/2) + 2A(n/2) + 2*Sum_{k=0..(n-2)/2}...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 935 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static void A359020_addint(seq_t *x, int d)
{
    if (d >= 0) *x += (seq_t)d;
    else *x -= (seq_t)(-d);
}

/* normalise s (any int) into r in 0..3 and add the carry (s - r)/4 to *q */
static void A359020_norm(int s, seq_t *q, int *r)
{
    int rr = ((s % 4) + 4) % 4;
    A359020_addint(q, (s - rr) / 4);
    *r = rr;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)13;

    size_t sz = (size_t)n + 1;
    seq_t *Aq = mem_new(seq_t, sz);
    seq_t *Cq = mem_new(seq_t, sz);
    seq_t *Pq = mem_new(seq_t, sz);
    int *Ar = (int *)malloc(sz * sizeof(int));
    int *Cr = (int *)malloc(sz * sizeof(int));
    int *Pr = (int *)malloc(sz * sizeof(int));
    if (!Ar || !Cr || !Pr) abort();

    static const int A0[5] = {1, 1, 5, 13, 40};
    static const int C0[4] = {1, 1, 3, 5};

    for (size_t i = 0; i < sz; ++i)
    {
        /* A054856 */
        if (i < 5)
        {
            Aq[i] = (seq_t)(A0[i] / 4);
            Ar[i] = A0[i] % 4;
        }
        else
        {
            seq_t q = 2 * Aq[i-1] + 3 * Aq[i-2] - Aq[i-4] - Aq[i-5];
            int s = 2 * Ar[i-1] + 3 * Ar[i-2] - Ar[i-4] - Ar[i-5];
            A359020_norm(s, &q, &Ar[i]);
            Aq[i] = q;
        }
        /* compo */
        if (i < 4)
        {
            Cq[i] = (seq_t)(C0[i] / 4);
            Cr[i] = C0[i] % 4;
        }
        else
        {
            seq_t q = Cq[i-1] + 2 * Cq[i-2] + Cq[i-4];
            int s = Cr[i-1] + 2 * Cr[i-2] + Cr[i-4];
            A359020_norm(s, &q, &Cr[i]);
            Cq[i] = q;
        }
        /* prefix sums of A054856 */
        if (i == 0)
        {
            Pq[0] = Aq[0];
            Pr[0] = Ar[0];
        }
        else
        {
            seq_t q = Pq[i-1] + Aq[i];
            int s = Pr[i-1] + Ar[i];
            A359020_norm(s, &q, &Pr[i]);
            Pq[i] = q;
        }
    }

    seq_t q = 0;
    int r = 0;
    size_t N = (size_t)n;
    q += Aq[N]; r += Ar[N];
    q += Cq[N]; r += Cr[N];
    if ((N & 1) == 0)
    {
        q += 4 * Aq[(N - 2) / 2]; r += 4 * Ar[(N - 2) / 2];
        q += 2 * Aq[(N - 4) / 2]; r += 2 * Ar[(N - 4) / 2];
        q += 2 * Aq[N / 2];       r += 2 * Ar[N / 2];
        q += 2 * Pq[(N - 2) / 2]; r += 2 * Pr[(N - 2) / 2];
    }
    else
    {
        q += 2 * Aq[(N - 3) / 2]; r += 2 * Ar[(N - 3) / 2];
        q += 2 * Aq[(N - 1) / 2]; r += 2 * Ar[(N - 1) / 2];
        q += 2 * Pq[(N - 3) / 2]; r += 2 * Pr[(N - 3) / 2];
    }
    seq_t res = q + (seq_t)(r / 4);

    mem_free(Aq);
    mem_free(Cq);
    mem_free(Pq);
    free(Ar);
    free(Cr);
    free(Pr);
    return res;
}
