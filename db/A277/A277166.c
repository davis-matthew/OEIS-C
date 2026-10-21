/*
 * A277166  Numbers m such that m divides the number of divisors of m!!.
 * https://oeis.org/A277166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer m such that m divides τ(m!!)
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Parameters: enlarge as needed; for n≈10000 the bound 2 000 000 is safe   */
#define MAXM 2000000          /* upper limit for m  */
#define SEQ_CAP 20000         /* capacity of the sequence storage */

/* --------------------------------------------------------------------- */
/* Sieve for smallest prime factor up to MAXM */
static uint32_t spf[MAXM + 1];

/* --------------------------------------------------------------------- */
/* Separate tables for odd and even parity of m!!                         */
/* exps_*  : exponent of each prime in the current m!!                    */
/* exp_d_* : exponent of each prime in τ(m!!)                                */
static uint32_t exps_odd[MAXM + 1];
static uint32_t exp_d_odd[MAXM + 1];
static uint32_t exps_even[MAXM + 1];
static uint32_t exp_d_even[MAXM + 1];

/* --------------------------------------------------------------------- */
/* Storage for the sequence values                                          */
static seq_t seq[SEQ_CAP];
static size_t seq_len = 0;         /* number of terms stored */

/* --------------------------------------------------------------------- */
/* State for incremental generation                                        */
static idx_t next_m = 1;           /* next m to be processed          */

/* --------------------------------------------------------------------- */
/* Build the linear sieve for smallest prime factor                         */
static void build_spf(void) {
    for (idx_t i = 2; i <= MAXM; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= MAXM; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Adjust exp_d_arr by factoring n and adding delta * multiplicity         */
static void adjust_factor(uint32_t n, int delta, uint32_t *exp_d_arr) {
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t cnt = 0;
        while (n % p == 0) { n /= p; ++cnt; }
        /* delta is ±1, cnt ≥ 1 */
        int32_t mult = delta * (int32_t)cnt;
        int32_t val = (int32_t)exp_d_arr[p] + mult;
        exp_d_arr[p] = (uint32_t)val; /* guaranteed non‑negative */
    }
}

/* --------------------------------------------------------------------- */
/* Main function                                                           */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        build_spf();              /* one‑time sieve */
        initialized = true;
    }

    /* generate sequence terms until we have the n‑th term           */
    while (seq_len < (size_t)n) {
        /* ----------------------------------------------------------------- */
        /* Process next m                                                               */
        idx_t m = next_m++;
        uint32_t temp = (uint32_t)m;                  /* m ≤ MAXM */

        /* factor m ------------------------------------------------------------- */
        uint32_t primes_buf[32];
        uint32_t exps_buf[32];
        size_t pf_cnt = 0;
        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t cnt = 0;
            while (temp % p == 0) { temp /= p; ++cnt; }
            primes_buf[pf_cnt]   = p;
            exps_buf[pf_cnt]     = cnt;
            ++pf_cnt;
        }

        /* ----------------------------------------------------------------- */
        /* Choose parity tables                                                   */
        uint32_t *exp_arr;
        uint32_t *exp_d_arr;
        if (m & 1U) {                /* odd */
            exp_arr  = exps_odd;
            exp_d_arr = exp_d_odd;
        } else {                     /* even */
            exp_arr  = exps_even;
            exp_d_arr = exp_d_even;
        }

        /* ----------------------------------------------------------------- */
        /* Update exponent tables for primes dividing m                        */
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint32_t p  = primes_buf[i];
            uint32_t e  = exps_buf[i];

            uint32_t old_e = exp_arr[p];
            uint32_t new_e = old_e + e;
            exp_arr[p] = new_e;

            /* adjust τ(m!!) by removing old_e+1 and adding new_e+1           */
            adjust_factor(old_e + 1U, -1, exp_d_arr);
            adjust_factor(new_e + 1U,  1, exp_d_arr);
        }

        /* ----------------------------------------------------------------- */
        /* Test divisibility m | τ(m!!)                                         */
        bool ok = true;
        for (size_t i = 0; i < pf_cnt && ok; ++i) {
            uint32_t p = primes_buf[i];
            uint32_t fq = exps_buf[i];
            if (exp_d_arr[p] < fq) ok = false;
        }

        if (ok) {                     /* store the term */
            if (seq_len < SEQ_CAP)
                seq[seq_len++] = (seq_t)m;
            /* safety: if seq_len were to exceed SEQ_CAP, we truncate */
        }
    }

    return seq[n - 1];
}
