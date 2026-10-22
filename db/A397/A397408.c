/*
 * A397408  Shortest path indices during walk over Archimedean spiral lattice
 *          with divergence angle equal to golden angle and starting at the
 *          center.
 * https://oeis.org/A397408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The function implements the definition of OEIS A397408:
 * a(0)=0. For n>0 the walk starts at the point with k=0 and each step
 * moves to the nearest unvisited lattice point
 * (k·cos(k·θ), k·sin(k·θ)) with θ = 2π·(1−1/φ) (golden angle),
 * where φ=(1+√5)/2.  a(n) is the index k of the point reached after
 * n such steps.
 * The algorithm precomputes a fixed number of lattice points and
 * simulates the walk, caching results so that arbitrary calls to a(n)
 * (in any order) are answered quickly.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

/* Adjust MAX_K if larger search space is needed. */
#define MAX_K   20000

seq_t a(idx_t n)
{
    /* Static state shared across calls */
    static bool *visited    = NULL;
    static double *xcoord   = NULL;
    static double *ycoord   = NULL;
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed  = 0;      /* number of terms already computed
                                         (a(0) … a(computed-1)) */
    static idx_t  cur_idx   = 0;      /* index of the current lattice point */
    static bool   initialized = false;

    /* One‑time initialization */
    if (!initialized) {
        /* Allocate visited and coordinate arrays */
        visited  = (bool*)calloc(MAX_K, sizeof(bool));
        xcoord   = (double*)malloc(MAX_K * sizeof(double));
        ycoord   = (double*)malloc(MAX_K * sizeof(double));

        /* Compute golden angle */
        const double phi   = (1.0 + sqrt(5.0)) / 2.0;
        const double pi    = acos(-1.0);
        const double theta = 2.0 * pi * (1.0 - 1.0 / phi);

        /* Pre‑compute lattice points (k·cos(k·θ), k·sin(k·θ)) */
        for (idx_t k = 0; k < MAX_K; ++k) {
            double ang  = k * theta;
            xcoord[k]   = k * cos(ang);
            ycoord[k]   = k * sin(ang);
        }

        /* Allocate initial cache */
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));

        /* Prepare walk */
        visited[0] = true;
        cur_idx    = 0;
        cache[0]   = (seq_t)0;
        computed   = 1;          /* a(0) already stored */
        initialized = true;
    }

    /* Grow cache as needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to requested n */
    while (computed <= n) {
        idx_t best_k      = 0;
        double best_dist2 = 1e300;  /* sufficiently large initial distance */

        /* Brute‑force search for closest unvisited point */
        for (idx_t k = 0; k < MAX_K; ++k) {
            if (visited[k]) continue;
            double dx = xcoord[cur_idx] - xcoord[k];
            double dy = ycoord[cur_idx] - ycoord[k];
            double d2 = dx * dx + dy * dy;
            if (d2 < best_dist2) {
                best_dist2 = d2;
                best_k = k;
            }
        }

        /* Step to the chosen point */
        visited[best_k] = true;
        cur_idx = best_k;
        cache[computed] = (seq_t)best_k;
        ++computed;
    }

    return cache[n];
}
