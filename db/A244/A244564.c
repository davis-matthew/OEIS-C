/*
 * A244564  Odd integers n such that for every integer k>0, n * 2^k + 1 has a
 *          divisor in the set { 3, 5, 7, 13, 19, 73, 109 }.
 * https://oeis.org/A244564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 144, u64 n <= 144, u128 n <= 144,
 *            big any n
 * Helpers:   pow_mod_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244564 – odd integers n such that for every integer k>0,
   n * 2^k + 1 has a divisor in {3,5,7,13,19,73,109}.
   The sequence satisfies the recurrence

        a(n + 144) = a(n) + 412729590   for all n ≥ 1

   The first 144 terms are precomputed by a search; thereafter the
   recurrence is applied.
   The function below implements this algorithm; it is correct for
   any seq_t and idx_t types provided by the build. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_mod_int.h"

/* ------------------ magic constants ------------------ */
#define NUM_PRIMES 7
#define MAX_PRIME    109

static const int primes[NUM_PRIMES] = {3,5,7,13,19,73,109};

static int orders[NUM_PRIMES];
static struct BitMask {
    uint64_t w[4];
} mask_tbl[NUM_PRIMES][MAX_PRIME];
static struct BitMask full_mask;
static uint64_t L;                /* lcm of the orders */

/* first 144 solution values */
static seq_t base_vals[144];
static int base_filled = 0;

/* helpers ------------------------------------------------ */

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = b; b = a % b; a = t; }
    return a;
}
static uint64_t lcm_u64(uint64_t a, uint64_t b) {
    return a / gcd_u64(a,b) * b;
}

/* ----------------------------------------------------- */
/* build the tables and precomputed terms on first call */
static void init_tables(void) {
    if (base_filled) return;      /* already initialised */

    /* ---- compute orders and L ----------------------------------- */
    L = 1;
    for (int i = 0; i < NUM_PRIMES; ++i) {
        int p = primes[i];
        int val = 2 % p;
        int ord = 1;
        while (val != 1) {
            val = (val * 2) % p;
            ++ord;
        }
        orders[i] = ord;
        L = lcm_u64(L, (uint64_t)ord);
    }

    /* ---- build full mask --------------------------------------- */
    for (int wi = 0; wi < 4; ++wi) full_mask.w[wi] = 0ULL;
    for (uint64_t k = 0; k < L; ++k) {
        int wi = (int)(k >> 6);
        int bi = (int)(k & 63);
        full_mask.w[wi] |= (1ULL << bi);
    }

    /* ---- build mask tables ------------------------------------- */
    for (int i = 0; i < NUM_PRIMES; ++i) {
        int p = primes[i];
        int ord = orders[i];

        /* multiplicative inverse table for 1..p-1 */
        int inv[MAX_PRIME] = {0};
        for (int x = 1; x < p; ++x) inv[x] = pow_mod_int(x, p-2, p);

        /* exponent of each value that is a power of two modulo p */
        int exp_of_val[MAX_PRIME];
        for (int v = 0; v < MAX_PRIME; ++v) exp_of_val[v] = -1;
        int pow = 1 % p;
        for (int e = 0; e < ord; ++e) {
            exp_of_val[pow] = e;
            pow = (pow * 2) % p;
        }

        /* build mask for each residue r = n mod p */
        for (int r = 0; r < p; ++r) {
            struct BitMask bm = {0ULL,0ULL,0ULL,0ULL};
            if (r != 0) {
                int inv_r  = inv[r];                     /* r*inv_r ≡ 1 (mod p) */
                int r_neg  = (p - inv_r) % p;             /* -inv_r (mod p) */
                int t0     = exp_of_val[r_neg];           /* exponent giving this residue */
                if (t0 != -1) {
                    for (uint64_t k = (uint64_t)t0; k < L; k += (uint64_t)ord) {
                        int wi = (int)(k >> 6);
                        int bi = (int)(k & 63);
                        bm.w[wi] |= (1ULL << bi);
                    }
                }
            }
            mask_tbl[i][r] = bm;
        }
    }

    /* ---- find the first 144 solution values --------------------- */
    uint64_t cand = 1;                     /* first odd candidate */
    int residues[NUM_PRIMES];
    for (int i = 0; i < NUM_PRIMES; ++i) residues[i] = cand % primes[i];

    while (base_filled < 144) {
        /* combine masks of all primes */
        struct BitMask cur = {0ULL,0ULL,0ULL,0ULL};
        for (int i = 0; i < NUM_PRIMES; ++i) {
            const struct BitMask *pm = &mask_tbl[i][residues[i]];
            cur.w[0] |= pm->w[0];
            cur.w[1] |= pm->w[1];
            cur.w[2] |= pm->w[2];
            cur.w[3] |= pm->w[3];
        }

        /* test if all bits are set */
        if ( (cur.w[0] == full_mask.w[0]) &&
             (cur.w[1] == full_mask.w[1]) &&
             (cur.w[2] == full_mask.w[2]) &&
             (cur.w[3] == full_mask.w[3]) )
        {
            base_vals[base_filled++] = (seq_t)cand;
        }

        /* go to next odd number */
        cand += 2;
        for (int i = 0; i < NUM_PRIMES; ++i) {
            int r = residues[i] + 2;
            if (r >= primes[i]) r -= primes[i];
            residues[i] = r;
        }
    }
}

/* ---------------------------------------------------------- */
seq_t a(idx_t n) {
    init_tables();                /* ensure tables and base values are ready */

    if (n <= 0) return 0;          /* undefined for index <= 0 */

    const seq_t STEP = (seq_t)412729590ULL;

    idx_t idx   = n - 1;
    idx_t which = idx % 144;
    idx_t block = idx / 144;

    return base_vals[which] + STEP * (seq_t)block;
}
