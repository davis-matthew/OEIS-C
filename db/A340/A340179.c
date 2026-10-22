/*
 * A340179  a(n) = Sum_{x in C(n)} (A023896(n) mod x), where C(n) is the set
 *          of numbers < n coprime to n, and A023896(n) is the sum of C(n).
 * https://oeis.org/A340179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{x in C(n)} (c mod x), where C(n) = { 1 ≤ x < n |
 *            gcd(x,n)=1 } and c = Sum_{x in C(n)} x = n*phi(n)/2
 * Range:     u8 n <= 28, u16 n <= 528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- helpers ---------- */
static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ----- smallest prime factor table ----- */
#define MAX_SPF 1000000
static uint32_t *spf = NULL;

static void init_spf(void) {
    if (spf) return;
    spf = (uint32_t *)malloc((MAX_SPF + 1) * sizeof(uint32_t));
    if (!spf) return;   /* allocation failure; subsequent calls will fail silently */
    for (idx_t i = 0; i <= MAX_SPF; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= MAX_SPF; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= MAX_SPF) {
                for (idx_t j = i * i; j <= MAX_SPF; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ---------- main function ---------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but safe return 0 */

    /* Ensure SPF table is ready */
    init_spf();

    /* phi(n) */
    idx_t nn = n;
    idx_t phi_val = n;
    while (nn > 1) {
        idx_t p = spf[nn];
        if (p == 0) p = nn;    /* nn is prime > MAX_SPF */
        while (nn % p == 0) nn /= p;
        phi_val = phi_val * (p - 1) / p;
    }

    /* c = n * phi(n) / 2 */
    seq_t c = (seq_t)n * (seq_t)phi_val / 2;

    seq_t sum = 0;
    for (idx_t x = 1; x < n; ++x) {
        if (gcd_idx(x, n) == 1) {
            sum += c % (seq_t)x;
        }
    }
    return sum;
}
