/*
 * A381640  Numbers k such that f(k) > f(m) for all m < k, where f(k) =
 *          A381638(k)/A381639(k).
 * https://oeis.org/A381640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k with f(k) > f(m) for all m < k, f =
 *            A381638/A381639 = Sum_{i<omega(k)} p_i/p_{i+1}; computed by
 *            branch-and-bound search over squarefree numbers for the next
 *            record
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A381640_MAXREC 256

static seq_t A381640_recs[A381640_MAXREC];
static long double A381640_fv[A381640_MAXREC];
static size_t A381640_nrec = 0;

static uint64_t *A381640_pl = NULL;
static size_t A381640_pn = 0, A381640_pcap = 0;

static uint64_t A381640_prime(size_t i)
{
    while (A381640_pn <= i) {
        uint64_t c;
        if (A381640_pn == A381640_pcap) {
            size_t nc = A381640_pcap ? A381640_pcap * 2 : 1024;
            A381640_pl = (uint64_t *)realloc(A381640_pl, nc * sizeof(uint64_t));
            A381640_pcap = nc;
        }
        c = A381640_pn ? A381640_pl[A381640_pn - 1] + 1 : 2;
        for (;; c++) {
            bool ok = true;
            for (size_t j = 0; j < A381640_pn; j++) {
                uint64_t p = A381640_pl[j];
                if (p * p > c) break;
                if (c % p == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        A381640_pl[A381640_pn++] = c;
    }
    return A381640_pl[i];
}

static seq_t A381640_best;
static long double A381640_f0;

static void A381640_dfs(seq_t prod, uint64_t last, long double sum, size_t idx)
{
    const long double eps = 1e-15L;
    for (size_t i = idx;; i++) {
        uint64_t q = A381640_prime(i);
        seq_t R = (A381640_best - 1) / prod;
        if ((seq_t)q > R) break;
        int t = 0;
        {
            seq_t r = R;
            while (r >= (seq_t)q) { r /= (seq_t)q; t++; }
        }
        long double ratio = last ? (long double)last / (long double)q : 0.0L;
        if (sum + ratio + (long double)(t - 1) <= A381640_f0 + eps) break;
        long double ns = sum + ratio;
        seq_t np = prod * (seq_t)q;
        if (last && ns > A381640_f0 + eps) {
            if (np < A381640_best) A381640_best = np;
        } else {
            A381640_dfs(np, q, ns, i + 1);
        }
    }
}

static void A381640_next(void)
{
    seq_t k0 = A381640_recs[A381640_nrec - 1];
    long double f0 = A381640_fv[A381640_nrec - 1];
    seq_t L = k0 * 2 + 8;
    for (;;) {
        A381640_best = L + 1;
        A381640_f0 = f0;
        A381640_dfs((seq_t)1, 0, 0.0L, 0);
        if (A381640_best <= L) break;
        L = L * 2;
    }
    /* compute f of the found number from its prime factors */
    seq_t m = A381640_best;
    long double s = 0.0L;
    uint64_t prev = 0;
    for (size_t i = 0; m > 1; i++) {
        uint64_t p = A381640_prime(i);
        if (m % (seq_t)p == 0) {
            while (m % (seq_t)p == 0) m /= (seq_t)p;
            if (prev) s += (long double)prev / (long double)p;
            prev = p;
        }
    }
    A381640_recs[A381640_nrec] = A381640_best;
    A381640_fv[A381640_nrec] = s;
    A381640_nrec++;
}

seq_t a(idx_t n)
{
    if (A381640_nrec == 0) {
        A381640_recs[0] = 1;
        A381640_fv[0] = 0.0L;
        A381640_nrec = 1;
    }
    while (A381640_nrec < (size_t)n && A381640_nrec < A381640_MAXREC)
        A381640_next();
    return A381640_recs[(size_t)n - 1];
}
