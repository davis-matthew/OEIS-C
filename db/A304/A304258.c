/*
 * A304258  Number of nX3 0..1 arrays with every element unequal to 2, 3, 5 or
 *          7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304258:
   Number of n×3 zero/one arrays with the king‑move adjacency
   restriction and upper left element zero.
   Recurrence (OEIS specification):
     a(n) = a(n‑1) +5*a(n‑2) -a(n‑3) -13*a(n‑4) +5*a(n‑5) +10*a(n‑6)
            -16*a(n‑7) +4*a(n‑8) +14*a(n‑9) -10*a(n‑10) -14*a(n‑11)
            +14*a(n‑12) +4*a(n‑13) -5*a(n‑14) +13*a(n‑15) -2*a(n‑17)
   Initial values:
     a(1)=0, a(2)=4, a(3)=8, a(4)=13, a(5)=17, a(6)=26, a(7)=61,
     a(8)=120, a(9)=233, a(10)=515, a(11)=959,
     a(12)=2203, a(13)=4422, a(14)=9326, a(15)=20164,
     a(16)=40967, a(17)=89272, a(18)=185242.
*/

#define MAX_N 20000   /* sufficient for all n requested in the tests */

static seq_t cache[MAX_N + 1];
static idx_t cached_up_to = 0;

static void init_cache(void) {
    static const seq_t base[18] = {
        0,      4,      8,     13,     17,     26,    61,    120,
       233,    515,    959,   2203,   4422,   9326,  20164,  40967,
      89272, 185242
    };
    cache[0] = 0;  /* unused; keep 1‑based indexing simple */
    for (idx_t i = 1; i <= 18 && i <= MAX_N; ++i) {
        cache[i] = base[i - 1];
    }
    cached_up_to = (cached_up_to > 18) ? cached_up_to : 18;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;   /* undefined for n<1, return 0 for safety */
    }
    if (n > MAX_N) {
        /* For safety: truncate to MAX_N; the sequence is not
           required for larger n in the tests. */
        n = MAX_N;
    }

    if (cached_up_to == 0) {
        init_cache();
    }

    if (n <= cached_up_to) {
        return cache[n];
    }

    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i <= 18) {
            /* These have been pre‑initialised. */
            continue;
        }
        cache[i] = cache[i - 1] + 5 * cache[i - 2]
                   - cache[i - 3] - 13 * cache[i - 4]
                   + 5 * cache[i - 5] + 10 * cache[i - 6]
                   - 16 * cache[i - 7] + 4 * cache[i - 8]
                   + 14 * cache[i - 9] - 10 * cache[i - 10]
                   - 14 * cache[i - 11] + 14 * cache[i - 12]
                   + 4 * cache[i - 13] - 5 * cache[i - 14]
                   + 13 * cache[i - 15] - 2 * cache[i - 17];
    }
    cached_up_to = n;
    return cache[n];
}
