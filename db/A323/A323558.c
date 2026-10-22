/*
 * A323558  Numbers k such that n^2 + n + k has no prime factor p <= 41 for
 *          any integer n.
 * https://oeis.org/A323558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323558: Numbers k such that n^2 + n + k has no prime factor
   <= 41 for any integer n. */
#include <stdbool.h>
#include <stddef.h>

/* primes up to 41, excluding 2 because k must be odd */
static const int PRIMES[] = {3,5,7,11,13,17,19,23,29,31,37,41};
#define NUM_PRIMES (sizeof(PRIMES)/sizeof(PRIMES[0]))

/* forbidden residue tables: forb[i][r] == true if k ≡ r (mod PRIMES[i])
   causes 1 - 4k to be a quadratic residue mod PRIMES[i], i.e. n^2+n+k divisible
   by PRIMES[i] for some integer n. */
static bool forb[NUM_PRIMES][42];
static bool initialized = false;

static void init(void)
{
    if (initialized) return;
    for (size_t pi = 0; pi < NUM_PRIMES; ++pi) {
        int p = PRIMES[pi];
        bool isQR[42] = {0};
        /* mark quadratic residues modulo p */
        for (int r = 0; r < p; ++r) {
            int sq = (r * r) % p;
            isQR[sq] = true;
        }
        /* determine forbidden k modulo p */
        for (int kmod = 0; kmod < p; ++kmod) {
            int t = (1 + p - ((4 * kmod) % p)) % p;   /* t = (1 - 4k) mod p */
            forb[pi][kmod] = isQR[t];
        }
    }
    initialized = true;
}

/* Returns the nth term (1-indexed) of the sequence. */
seq_t a(idx_t n)
{
    init();
    seq_t cur = (seq_t)1;           /* start with k = 1 (odd numbers only) */
    int km[NUM_PRIMES];
    for (size_t i = 0; i < NUM_PRIMES; ++i)
        km[i] = (int)(cur % PRIMES[i]);   /* initial k mod p */

    idx_t count = 0;
    for (;;) {
        bool good = true;
        for (size_t i = 0; i < NUM_PRIMES; ++i) {
            if (forb[i][km[i]]) { good = false; break; }
        }
        if (good) {
            ++count;
            if (count == n) return cur;
        }
        /* advance to next odd k */
        cur = (seq_t)((unsigned long long)cur + 2);
        for (size_t i = 0; i < NUM_PRIMES; ++i) {
            km[i] += 2;
            if (km[i] >= PRIMES[i]) km[i] -= PRIMES[i];
        }
    }
}
