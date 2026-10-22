/*
 * A371517  G.f. A(x) satisfies A(x) = (1 + x*A(x) / (1-x))^4.
 * https://oeis.org/A371517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) follows the 4‑term recurrence 283*n*(n+2)*(n+1)*a(n) -
 *            12*(73*n+251)*(n+2)*(n+1)*a(n+1) +
 *            6*(n+2)*(155*n^2+1213*n+2384)*a(n+2) -
 *            4*(n+4)*(91*n^2+824*n+1869)*a(n+3) +
 *            3*(n+5)*(3*n+14)*(3*n+16)*a(n+4) = 0 with a(0)=1, a(1)=4,
 *            a(2)=26, a(3)=188. For general n >= 0 the function returns a(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t len = 0;
    static size_t cap = 0;

    /* Ensure base cases are stored */
    if (len == 0) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)26;
        cache[3] = (seq_t)188;
        len = 4;
        cap = 4;
    }
    if (n < len) return cache[n];

    /* Grow cache if needed */
    if (n >= cap) {
        size_t newcap = cap ? (cap * 2) : 4;
        if (newcap <= n) newcap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms */
    for (idx_t i = len; i <= n; ++i) {
        idx_t idx = i - 4;          /* N = i-4 */
        seq_t N = (seq_t)idx;

        seq_t aN   = cache[idx];
        seq_t aNp1 = cache[idx+1];
        seq_t aNp2 = cache[idx+2];
        seq_t aNp3 = cache[idx+3];

        /* Decide which arithmetic to use (cheap branches are cheap) */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* 64‑bit build: use unsigned __int128 for intermediates */
            typedef __int128 int128;
            int128 t1 = (int128)73 * (int128)N + 251;
            int128 part1 = (int128)12 * t1 * (int128)(N + 2) * (int128)(N + 1) * (int128)aNp1;

            int128 part2 = (int128)283 * (int128)N * (int128)(N + 2) * (int128)(N + 1) * (int128)aN;

            int128 Nsq = (int128)N * (int128)N;
            int128 t3fac = (int128)155 * Nsq + (int128)1213 * (int128)N + 2384;
            int128 part3 = (int128)6 * (int128)(N + 2) * t3fac * (int128)aNp2;

            int128 t4fac = (int128)91 * Nsq + (int128)824 * (int128)N + 1869;
            int128 part4 = (int128)4 * (int128)(N + 4) * t4fac * (int128)aNp3;

            int128 pos = part1 + part4;
            int128 neg = part2 + part3;
            int128 num = pos - neg;      /* positive as shown by recurrence */

            int128 den = (int128)3 * (int128)(N + 5) *
                         (int128)(3 * (int128)N + 14) *
                         (int128)(3 * (int128)N + 16);

            cache[i] = (seq_t)(num / den);
        } else {
            /* Big‑integer build: use seq_t operations directly */
            /* Compute coefficients */
            seq_t part1 = 12 * ((73 * N + (seq_t)251)) * (N + 2) * (N + 1);
            part1 *= aNp1;

            seq_t part2 = 283 * N * (N + 2) * (N + 1);
            part2 *= aN;

            seq_t Nsq = N * N;
            seq_t part3fac = 155 * Nsq + 1213 * N + 2384;
            seq_t part3 = 6 * (N + 2) * part3fac;
            part3 *= aNp2;

            seq_t part4fac = 91 * Nsq + 824 * N + 1869;
            seq_t part4 = 4 * (N + 4) * part4fac;
            part4 *= aNp3;

            seq_t positive = part1 + part4;
            seq_t negative = part2 + part3;
            seq_t num = positive - negative;

            seq_t den = 3 * (N + 5) * (3 * N + 14) * (3 * N + 16);

            cache[i] = num / den;
        }
    }
    len = n + 1;
    return cache[n];
}
