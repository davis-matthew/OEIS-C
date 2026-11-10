/*
 * A276767  Let A_n be the sequence defined in the same way as A159559 but
 *          with initial term prime(n), n>=2; a(n) is the smallest m such that
 *          for i>=2, A_n(i) - A_2(i) <= A_n(m) - A_2(m).
 * https://oeis.org/A276767
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m>=2 maximizing A_n(m)-A_2(m), where A_k is the
 *            A159559 rule started at prime(k); computed by direct simulation
 *            up to a fixed index bound
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 53 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define A276767_N 20000
#define A276767_L 4000000

static unsigned char *A276767_sv = NULL;
static long long A276767_base[A276767_N + 1];

static void A276767_init(void)
{
    if (A276767_sv) return;
    A276767_sv = (unsigned char *)malloc(A276767_L + 1);
    for (long long i = 0; i <= A276767_L; i++) A276767_sv[i] = 1;
    A276767_sv[0] = A276767_sv[1] = 0;
    for (long long i = 2; i * i <= A276767_L; i++)
        if (A276767_sv[i])
            for (long long j = i * i; j <= A276767_L; j += i) A276767_sv[j] = 0;
}

/* fill arr[2..N] for start value s; returns last valid index */
static int A276767_gen(long long s, long long *arr)
{
    arr[2] = s;
    for (int i = 3; i <= A276767_N; i++) {
        long long cur = arr[i - 1] + 1;
        bool need = A276767_sv[i] != 0;
        while (cur <= A276767_L && (A276767_sv[cur] != 0) != need) cur++;
        if (cur > A276767_L) return i - 1;
        arr[i] = cur;
    }
    return A276767_N;
}

seq_t a(idx_t n)
{
    static long long arr[A276767_N + 1];
    static bool have_base = false;
    static int base_last = 0;
    A276767_init();
    if (!have_base) {
        base_last = A276767_gen(3, A276767_base);
        have_base = true;
    }
    /* find prime(n) */
    long long cnt = 0, p = 0;
    for (long long v = 2; v <= A276767_L; v++) {
        if (A276767_sv[v] && ++cnt == (long long)n) { p = v; break; }
    }
    int last = A276767_gen(p, arr);
    if (last > base_last) last = base_last;
    long long best = arr[2] - A276767_base[2];
    int bi = 2;
    for (int i = 3; i <= last; i++) {
        long long r = arr[i] - A276767_base[i];
        if (r > best) { best = r; bi = i; }
    }
    return (seq_t)bi;
}
