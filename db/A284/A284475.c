/*
 * A284475  Total number of parts in all partitions of n into equal parts,
 *          minus the total number of parts in all partitions of n into
 *          consecutive parts.
 * https://oeis.org/A284475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(n) – A204217(n)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A000203(n)  = sum of divisors of n (σ(n)) */
/*  A204217(n)  = total number of parts in all partitions of n into consecutive parts */
/*  a(1) = 0, a(2) = 2, a(3) = 1, … (as per OEIS A284475) */

#include <stdint.h>
#include <stddef.h>

/* ---------- helper: factorisation by trial division ---------- */
static void
factorize(idx_t n, idx_t *pf, int *exps, size_t *cnt)
{
    size_t c = 0;
    idx_t m   = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pf[c]   = p;
            exps[c] = e;
            ++c;
        }
    }
    if (m > 1) {
        pf[c]   = m;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ---------- recursive enumeration of divisors ---------- */
static void
enumerate_divisors(const idx_t *pf, const int *exps, size_t cnt,
                   size_t idx, idx_t cur,
                   seq_t *sum, idx_t two_n, int flag)
{
    /* flag == 0 : accumulate all divisors (σ)   */
    /* flag == 1 : accumulate only those producing consecutive-partitions  */
    if (idx == cnt) {
        if (flag == 0) {
            *sum += (seq_t)cur;
        } else {
            idx_t d = cur;
            idx_t e = two_n / cur;
            if (e >= d + 1 && ((e - d + 1) & 1ULL) == 0) {
                *sum += (seq_t)d;
            }
        }
        return;
    }
    idx_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(pf, exps, cnt, idx + 1, cur * mult,
                           sum, two_n, flag);
        mult *= pf[idx];
    }
}

/* ---------- compute σ(n) ---------- */
static seq_t
sigma_of(idx_t n)
{
    idx_t pf[20];
    int   exps[20];
    size_t cnt;
    factorize(n, pf, exps, &cnt);
    seq_t sum = 0;
    enumerate_divisors(pf, exps, cnt, 0, 1, &sum, 0, 0);
    return sum;
}

/* ---------- compute sum of parts for consecutive partitions ---------- */
static seq_t
sum_consecutive(idx_t n)
{
    idx_t two_n = 2 * n;
    idx_t pf[20];
    int   exps[20];
    size_t cnt;
    factorize(two_n, pf, exps, &cnt);
    seq_t sum = 0;
    enumerate_divisors(pf, exps, cnt, 0, 1, &sum, two_n, 1);
    return sum;
}

/* ---------- public entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for indices < 1 */
    seq_t sigma = sigma_of(n);
    seq_t consec = sum_consecutive(n);
    return sigma - consec;
}
