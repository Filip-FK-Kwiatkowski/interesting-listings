import java.io.File

fun main() {
    val path = "path/to/the/results/dir"
    File(path)
        .walk()
        .maxDepth(1)
        .filter { it.name.contains("Legacy-League-0") }
        .flatMap(::extractPlayerResults)
        .groupingBy { it.nameAndSurname }
        .fold(MatchResult(0, 0, 0)) { acc, result -> acc + result.matchResult }
        .toList()
        .sortedByDescending { it.second.winPercentage }
        .apply {
            val nameAndSurnameWidth = this
                .map(Pair<String, *>::first)
                .maxOfOrNull { it.length } ?: 0

            forEachIndexed { index, playerResult ->
                val formattedPlace = "${index + 1}".padEnd(2)
                val (nameAndSurname, allResults) = playerResult
                val formattedWinPercentage = String.format("%.2f", allResults.winPercentage)
                println("$formattedPlace. ${nameAndSurname.padEnd(nameAndSurnameWidth)} : $allResults ($formattedWinPercentage%)")
            }
        }
}

fun extractPlayerResults(file: File) =
    file.useLines { lines ->
        lines
            .filter { it.startsWith("|") }
            .drop(2)
            .map { it.split("|") }
            .map { (_, _, nameAndSurname, result) ->
                PlayerResult(nameAndSurname.trim(), MatchResult.from(result))
            }
            .toList()
    }

data class PlayerResult(val nameAndSurname: String, val matchResult: MatchResult)

data class MatchResult(val wins: Int, val losses: Int, val draws: Int) {
    val totalMatches
        get() = wins + losses + draws

    val winPercentage
        get() = wins.toDouble() * 100 / totalMatches

    companion object {
        fun from(textResult: String) =
            textResult
                .replace("$", "")
                .split("-")
                .map(String::trim)
                .map(String::toInt)
                .let { MatchResult(it[0], it[1], it[2]) }
    }

    override fun toString(): String {
        return "$wins-$losses-$draws"
    }

    operator fun plus(other: MatchResult) =
        MatchResult(wins + other.wins, losses + other.losses, draws + other.draws)
}
