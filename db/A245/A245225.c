/*
 * A245225  Continued fraction expansion of the constant c in A245224; c =
 *          sup{f(n,1)}, where f(1,x) = x + 1 and thereafter f(n,x) = x + 1 if
 *          n is in A054385, else f(n,x) = 1/x.
 * https://oeis.org/A245225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th partial quotient (offset 0) of c = sup{f(n,1)};
 *            computed by search: x after a run of +1 steps is the continued
 *            fraction [k_m; k_{m-1}, ..., k_2, k_1+1] built from the reversed
 *            run lengths k_i of A054385 members, and c's digits come from the
 *            alternating-lexicographic maximum over all r...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A245225_M 60000
#define A245225_L 300

static unsigned char A245225_k[A245225_M + 2];
static unsigned char A245225_dig[A245225_L];
static int A245225_ready = 0;

static unsigned A245225_better(size_t m1, size_t m2)
{
    /* returns 1 if the reversed history ending at run m1 beats that at run m2 */
    int i;
    for (i = 0; i < A245225_L; i++) {
        unsigned char x = A245225_k[m1 - (size_t)i];
        unsigned char y = A245225_k[m2 - (size_t)i];
        if (x != y) {
            if ((i & 1) == 0) return x > y;
            return x < y;
        }
    }
    return 0;
}

static void A245225_init(void)
{
    const unsigned __int128 R = (unsigned __int128)15819767068693265ULL; /* e/(e-1) * 1e16 */
    const unsigned __int128 D = (unsigned __int128)10000000000000000ULL;
    uint64_t m = 1, j = 1;
    uint64_t v = (uint64_t)(((unsigned __int128)j * R) / D);
    size_t cnt = 0;
    unsigned cur = 0;
    size_t best = 0;
    size_t r;
    int i;

    while (cnt < A245225_M) {
        if (m == v) {
            cur++;
            j++;
            v = (uint64_t)(((unsigned __int128)j * R) / D);
        } else {
            if (cur > 0) {
                cnt++;
                A245225_k[cnt] = (unsigned char)cur;
            }
            cur = 0;
        }
        m++;
    }
    for (r = (size_t)A245225_L + 2; r <= cnt; r++) {
        if (best == 0 || A245225_better(r, best)) best = r;
    }
    for (i = 0; i < A245225_L; i++)
        A245225_dig[i] = A245225_k[best - (size_t)i];
    A245225_ready = 1;
}

seq_t a(idx_t n)
{
    if (!A245225_ready) A245225_init();
    if (n >= (idx_t)A245225_L) return (seq_t)0;
    return (seq_t)A245225_dig[(size_t)n];
}
