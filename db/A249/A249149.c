/*
 * A249149  One more than positions of records in A249442.
 * https://oeis.org/A249149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A249440(n)+1, where A249440 lists the positions N of
 *            records in A249442 (A249442(N) = least m with binomial(N,m) not
 *            squarefree, 0 if none). Computed by a search over N using
 *            Kummer's theorem (borrows in base p) with a residue-wheel filter
 *            on 2,3,5
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A249149_M 1036800u /* 512*81*25 */

static uint32_t *A249149_primes = NULL;
static size_t A249149_np = 0;
static uint64_t A249149_pos[64];
static int A249149_cnt = 0;
static uint64_t A249149_next = 1;
static uint64_t A249149_r = 0;
static uint32_t *A249149_L = NULL;
static size_t A249149_Ln = 0;
static int64_t A249149_Lr = -1;

static void A249149_init_primes(void)
{
    if (A249149_primes) return;
    const uint32_t lim = 1u << 17;
    char *s = (char *)calloc(lim + 1, 1);
    A249149_primes = (uint32_t *)malloc(sizeof(uint32_t) * 13000);
    for (uint32_t i = 2; i <= lim; i++) {
        if (!s[i]) {
            A249149_primes[A249149_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
}

static int A249149_borrows(uint64_t N, uint64_t m, uint64_t p)
{
    int b = 0, carry = 0;
    while (N > 0) {
        uint64_t dn = N % p, dm = m % p;
        if (dn < dm + carry) { b++; carry = 1; if (b >= 2) return b; }
        else carry = 0;
        N /= p; m /= p;
        if (m == 0 && carry == 0) break;
    }
    return b;
}

static int A249149_lowbor(uint64_t x, uint64_t m, uint64_t p, int k)
{
    int b = 0, carry = 0;
    for (int i = 0; i < k; i++) {
        uint64_t dn = x % p, dm = m % p;
        if (dn < dm + carry) { b++; carry = 1; }
        else carry = 0;
        x /= p; m /= p;
    }
    return b;
}

/* true if binomial(N,m) squarefree for all 1<=m<=min(r,N) */
static bool A249149_clean(uint64_t N, uint64_t r)
{
    uint64_t mm = r < N ? r : N;
    for (size_t i = 0; i < A249149_np; i++) {
        uint64_t p = A249149_primes[i];
        if (p * p > N) break;
        if (p <= r) {
            for (uint64_t m = 1; m <= mm; m++)
                if (A249149_borrows(N, m, p) >= 2) return false;
        } else {
            if (N % p < r && (N / p) % p == 0) return false;
        }
    }
    return true;
}

static bool A249149_bad(uint64_t N, uint64_t m)
{
    for (size_t i = 0; i < A249149_np; i++) {
        uint64_t p = A249149_primes[i];
        if (p * p > N) break;
        if (A249149_borrows(N, m, p) >= 2) return true;
    }
    return false;
}

static uint64_t A249149_value(uint64_t N, uint64_t r)
{
    for (uint64_t m = r + 1; m <= N / 2; m++)
        if (A249149_bad(N, m)) return m;
    return 0;
}

static void A249149_table(uint64_t p, int k, uint64_t size, uint64_t r, char *ok)
{
    for (uint64_t x = 0; x < size; x++) {
        char good = 1;
        uint64_t mm = r < size - 1 ? r : size - 1;
        for (uint64_t m = 1; m <= mm; m++)
            if (A249149_lowbor(x, m, p, k) >= 2) { good = 0; break; }
        ok[x] = good;
    }
}

static void A249149_build_L(uint64_t r)
{
    static char ok2[512], ok3[81], ok5[25];
    A249149_table(2, 9, 512, r, ok2);
    A249149_table(3, 4, 81, r, ok3);
    A249149_table(5, 2, 25, r, ok5);
    if (!A249149_L) A249149_L = (uint32_t *)malloc(sizeof(uint32_t) * A249149_M);
    A249149_Ln = 0;
    for (uint32_t x = 0; x < A249149_M; x++)
        if (ok2[x & 511] && ok3[x % 81] && ok5[x % 25]) A249149_L[A249149_Ln++] = x;
    A249149_Lr = (int64_t)r;
}

static void A249149_extend(int need)
{
    A249149_init_primes();
    if (A249149_cnt == 0) {
        A249149_pos[0] = 0; A249149_cnt = 1; A249149_next = 1; A249149_r = 0;
    }
    while (A249149_cnt < need) {
        uint64_t N = A249149_next;
        if (N < A249149_M) {
            if (A249149_clean(N, A249149_r)) {
                uint64_t v = A249149_value(N, A249149_r);
                if (v > A249149_r) {
                    A249149_pos[A249149_cnt++] = N;
                    A249149_r = v;
                }
            }
            A249149_next = N + 1;
        } else {
            if (A249149_Lr != (int64_t)A249149_r) A249149_build_L(A249149_r);
            uint64_t base = (N / A249149_M) * (uint64_t)A249149_M;
            bool found = false;
            for (size_t i = 0; i < A249149_Ln; i++) {
                uint64_t c = base + A249149_L[i];
                if (c < A249149_next) continue;
                if (A249149_clean(c, A249149_r)) {
                    uint64_t v = A249149_value(c, A249149_r);
                    if (v > A249149_r) {
                        A249149_pos[A249149_cnt++] = c;
                        A249149_r = v;
                        A249149_next = c + 1;
                        found = true;
                        break;
                    }
                }
            }
            if (!found) A249149_next = base + A249149_M;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > 60) return 0;
    if ((int)n > A249149_cnt) A249149_extend((int)n);
    return (seq_t)(A249149_pos[n - 1] + 1);
}
