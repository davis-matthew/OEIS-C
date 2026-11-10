/*
 * A255363  Numbers with the property that A006255(k) = A070229(k).
 * https://oeis.org/A255363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th k with A006255(k) = A070229(k) = k + gpf(k);
 *            for each k not divisible by gpf(k)^2, run the GF(2)
 *            exponent-parity elimination over m = k+1..k+gpf(k), and keep k
 *            iff the first dependency occurs exactly at m = k+gpf(k)
 * Range:     u8 n <= 156, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 79 terms through n = 4865 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int A255363_reduce(uint64_t *v, int tw, uint64_t **basis) {
    int w = tw;
    while (w >= 0) {
        if (!v[w]) { w--; continue; }
        int b = 63 - __builtin_clzll(v[w]);
        int h = w * 64 + b;
        if (!basis[h]) return h;
        {
            const uint64_t *bv = basis[h];
            int i;
            for (i = 0; i <= w; i++) v[i] ^= bv[i];
        }
    }
    return -1;
}

/* builds parity vector of x in v, returns top word index */
static int A255363_setvec(uint64_t x, const uint32_t *spf, const int *pidx, uint64_t *v) {
    int idxs[24];
    int cnt = 0, hi = 0, i;
    while (x > 1) {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (e & 1) {
            int k = pidx[p];
            idxs[cnt++] = k;
            if (k > hi) hi = k;
        }
    }
    {
        int tw = hi >> 6;
        for (i = 0; i <= tw; i++) v[i] = 0;
        for (i = 0; i < cnt; i++) v[idxs[i] >> 6] ^= (uint64_t)1 << (idxs[i] & 63);
        return tw;
    }
}

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx;
    uint64_t L = 8 * n + 200;
    for (;;) {
        uint64_t i, j, k, count = 0, result = 0;
        bool overflow = false;
        uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
        int *pidx = (int *)calloc((size_t)(L + 1), sizeof(int));
        int P = 0, W;
        uint64_t *vn, *vm, *t, *pool;
        uint64_t **basis;
        int *pivlist;
        for (i = 2; i <= L; i++) {
            if (spf[i] == 0) {
                pidx[i] = P++;
                for (j = i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        W = (P + 63) / 64 + 1;
        vn = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
        vm = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
        t = (uint64_t *)malloc((size_t)W * sizeof(uint64_t));
        pool = (uint64_t *)malloc((size_t)P * (size_t)W * sizeof(uint64_t));
        basis = (uint64_t **)calloc((size_t)P + 1, sizeof(uint64_t *));
        pivlist = (int *)malloc(((size_t)P + 1) * sizeof(int));

        for (k = 2; count < n; k++) {
            uint64_t x = k, p = 0, m;
            int used = 0, tn, q;
            bool member = false;
            if (2 * k > L) { overflow = true; break; }
            while (x > 1) {
                uint32_t f = spf[x];
                p = f;
                x /= f;
            }
            if (k % (p * p) == 0) continue;
            tn = A255363_setvec(k, spf, pidx, vn);
            for (m = k + 1; m <= k + p; m++) {
                int tm = A255363_setvec(m, spf, pidx, vm);
                int tt = tm > tn ? tm : tn, h;
                for (q = 0; q <= tt; q++)
                    t[q] = (q <= tn ? vn[q] : 0) ^ (q <= tm ? vm[q] : 0);
                if (A255363_reduce(t, tt, basis) < 0) {
                    member = (m == k + p);
                    break;
                }
                if (m == k + p) break;
                h = A255363_reduce(vm, tm, basis);
                if (h >= 0) {
                    uint64_t *slot = pool + (size_t)used * (size_t)W;
                    for (q = 0; q <= tm; q++) slot[q] = vm[q];
                    basis[h] = slot;
                    pivlist[used++] = h;
                }
            }
            for (q = 0; q < used; q++) basis[pivlist[q]] = NULL;
            if (member) {
                count++;
                if (count == n) result = k;
            }
        }

        free(spf); free(pidx); free(vn); free(vm); free(t); free(pool);
        free(basis); free(pivlist);
        if (!overflow) return (seq_t)result;
        L *= 2;
    }
}
