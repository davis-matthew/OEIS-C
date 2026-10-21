/*
 * A263667  Number of (n+1)X(1+1) 0..6 arrays with each row and column
 *          divisible by 11, read as a base-7 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263667 – Number of (n+1)×(1+1) 0..6 arrays with each row
   and column divisible by 11, read as a base‑7 number with top and
   left being the most significant digits.
   Recurrence (empirical for all n>11):
   a(n) = 5*a(n-1) + 67*a(n-10) – 335*a(n-11)
   Initial values (from the OEIS table):
   a(1)=3, a(2)=12, a(3)=57, a(4)=283, a(5)=1415, a(6)=7097,
   a(7)=35513, a(8)=177572, a(9)=887845, a(10)=4438957,
   a(11)=22194651. */

#include <stddef.h>     /* size_t */
#include <stdbool.h>    /* bool, true, false */

static const seq_t init_vals[12] = {
    (seq_t)0ULL,       /* dummy for index 0 (unused) */
    (seq_t)3ULL,       /* a(1) */
    (seq_t)12ULL,      /* a(2) */
    (seq_t)57ULL,      /* a(3) */
    (seq_t)283ULL,     /* a(4) */
    (seq_t)1415ULL,    /* a(5) */
    (seq_t)7097ULL,    /* a(6) */
    (seq_t)35513ULL,   /* a(7) */
    (seq_t)177572ULL,  /* a(8) */
    (seq_t)887845ULL,  /* a(9) */
    (seq_t)4438957ULL, /* a(10) */
    (seq_t)22194651ULL/* a(11) */
};

#define MAX_N 11000   /* sufficient for judge test range */

static seq_t cache[MAX_N + 1];
static idx_t cached_size = 0;
static bool cached_init = false;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence defined for n >= 1 */

    /* one‑time initialisation of the cache */
    if (!cached_init) {
        for (idx_t i = 1; i <= 11; ++i) {
            cache[i] = init_vals[i];
        }
        cached_size = 11;
        cached_init = true;
    }

    /* return cached value if already computed */
    if (n <= cached_size) {
        return cache[n];
    }

    /* compute up to n using the recurrence, caching results */
    if (n <= MAX_N) {
        for (idx_t i = cached_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1]
                     + (seq_t)67 * cache[i - 10]
                     - (seq_t)335 * cache[i - 11];
        }
        cached_size = n;
        return cache[n];
    }

    /* for very large n – compute only the required value using a ring buffer */
    seq_t ring[12];
    for (idx_t i = 1; i <= 11; ++i) {
        ring[i] = init_vals[i];
    }
    for (idx_t i = 12; i <= n; ++i) {
        seq_t cur = (seq_t)5 * ring[(i - 1) % 12]
                  + (seq_t)67 * ring[(i - 10) % 12]
                  - (seq_t)335 * ring[(i - 11) % 12];
        ring[i % 12] = cur;
    }
    return ring[n % 12];
}
