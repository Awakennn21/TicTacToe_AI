#include "MCTS.h"
#include <algorithm>

int MCTS::RunMCTS(Node* state, size_t iterations, int playerToBeAsserted)
{
    srand(time(NULL));
    m_GameState.Clear();

    if (state == nullptr)
    {
        return MAP_SIZE / 2 * MAP_SIZE + MAP_SIZE / 2;
    }
    PlayerToBeAsserted = playerToBeAsserted;
    for (int i = 0; i < iterations; i++)
    {
        SelectNode(state);
    }
    AnalizeMoves(state);


    int Result = ReturnBestMove(state);
    Clear();
    return Result;
}

void MCTS::SelectNode(Node* start)
{
    Node* best_child = nullptr;
    while (!(start->children.empty()))
    {
        double best_score = -999;
        double score = -1.0;

        for (size_t i = 0; i < start->children.size(); i++)
        {
            if (start->children[i]->Visits != 0)
            {
                double exploitation_value = start->children[i]->WinsCount / start->children[i]->Visits;
                double exploration_value = (sqrt(log(start->Visits) / start->children[i]->Visits));
                score = exploitation_value + CONSTANT_C * exploration_value;
            }
            else
            {
                score = 9999999;
            }

            if (score > best_score)
            {
                best_score = score;
                best_child = start->children[i];
            }
        }

        start = best_child;
    }

    RecreatePos(start);
    if (!Evaluate().has_value())
    {
        if (start->Visits == 0)
        {
            RollOut(start);
        }
        else
        {
            Expansion(start);
        }
    }
    else
    {
        BackPropagate(start, Evaluate().value());
    }
}

void MCTS::AnalizeMoves(Node* root)
{
    for (auto current_node : m_AllAlocations)
    {
        RecreatePos(current_node);
        std::optional<double> StateResult = Evaluate();
        if (StateResult.has_value() && current_node->children.size() == 0)
        {
            if (StateResult.value() < 0.0)
            {
                current_node->Parent->WinPropability = StateResult.value() ;
                current_node->WinPropability = StateResult.value();
            }
        }
    }
}

void MCTS::RecreatePos(Node* start)
{
    m_GameState.Clear();
    while (true)
    {
        m_GameState.At(start->Move.pos / MAP_SIZE, start->Move.pos % MAP_SIZE, start->Move.player);
        if (start->Parent == nullptr)
        {
            return;
        }
        else
        {
            start = start->Parent;
        }
    }
}

void MCTS::Clear()
{
    for (auto i : m_AllAlocations)
    {
        delete i;
    }
    m_AllAlocations.clear();
}

size_t MCTS::EmptySpacesOnMap() const
{
    size_t count = 0;
    for (int i = 0; i < m_GameState.GetDimensions(); i++)
    {
        for (int j = 0; j < m_GameState.GetDimensions(); j++)
        {
            if (m_GameState.GetAt(i, j) == 0)
            {
                count++;
            }
        }
    }
    return count;
}

std::optional<double> MCTS::HasGameEnded(int y, int x)
{
    if (InRowCount(x, y, -1, -1) >= WIN_CONDITION ||
        InRowCount(x, y, -1, 0) >= WIN_CONDITION ||
        InRowCount(x, y, -1, 1) >= WIN_CONDITION ||
        InRowCount(x, y, 0, -1) >= WIN_CONDITION ||
        InRowCount(x, y, 0, 1) >= WIN_CONDITION ||
        InRowCount(x, y, 1, -1) >= WIN_CONDITION ||
        InRowCount(x, y, 1, 0) >= WIN_CONDITION ||
        InRowCount(x, y, 1, 1) >= WIN_CONDITION)
    {
        if (m_GameState.GetAt(y, x) == PlayerToBeAsserted)
        {
            return 1.0;
        }
        else
        {
            return -1.0;
        }
    }
    if (EmptySpacesOnMap() == 0)
    {
        return 0.0;
    }
    else
    {
        return {};
    }
}

void MCTS::Expansion(Node* node)
{
    std::vector<MoveInfo> valid_moves = GetValidMoves(node);
    for (const MoveInfo& move : valid_moves)
    {
        AddChildNode(node, move);
    }
    RecreatePos(node->children[0]);
    if (!Evaluate().has_value())
    {
        RollOut(node->children[0]);
    }
}

void MCTS::RollOut(Node* start)
{
    int WinsCount = 0;
    RecreatePos(start);
    int PlayerToMove = start->Move.player == 1 ? 2 : 1;
    std::vector<MoveInfo> SpacesAvailable = GetValidMoves(start);
    std::optional<double> Result;
    do
    {
        int Random = rand() % SpacesAvailable.size();

        m_GameState.At(SpacesAvailable[Random].pos / MAP_SIZE, SpacesAvailable[Random].pos % MAP_SIZE, PlayerToMove);
        Result = HasGameEnded(SpacesAvailable[Random].pos / MAP_SIZE, SpacesAvailable[Random].pos % MAP_SIZE);

        SpacesAvailable.erase(SpacesAvailable.begin() + Random);

        PlayerToMove = PlayerToMove == 1 ? 2 : 1;
    } while (!Result.has_value());
    WinsCount += Result.value();
    BackPropagate(start, WinsCount);
}

void MCTS::BackPropagate(Node* start, int wins)
{
    while (true)
    {
        start->WinsCount += wins;
        start->Visits++;
        start->WinPropability = std::clamp(start->WinsCount / start->Visits, -1.0, 1.0);
        if (start->Parent == nullptr)
        {
            return;
        }
        else
        {
            start = start->Parent;
        }
    }

}

std::optional<double> MCTS::Evaluate() const
{
    for (int y = 0; y < m_GameState.GetDimensions(); y++)
    {
        for (int x = 0; x < m_GameState.GetDimensions(); x++)
        {
            if (m_GameState.GetAt(y, x) != 0)
            {
                if (InRowCount(x, y, 1, 0) >= WIN_CONDITION ||
                    InRowCount(x, y, -1, 1) >= WIN_CONDITION ||
                    InRowCount(x, y, 0, 1) >= WIN_CONDITION ||
                    InRowCount(x, y, 1, 1) >= WIN_CONDITION)
                {
                    if (m_GameState.GetAt(y, x) == PlayerToBeAsserted)
                    {
                        return 1;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
        }
    }
    if (EmptySpacesOnMap() == 0)
    {
        return 0;
    }
    else
    {
        return {};
    }
}

int MCTS::ReturnBestMove(Node* root)
{
    Node* node = root;
    Node* best_child = root;
    double best_score = -999999;
    for (auto& child : node->children)
    {
        if (child->WinPropability > best_score)
        {
            best_child = child;
            best_score = child->WinPropability;
        }
        else if (child->WinPropability == best_score)
        {
            if (child->Visits > best_child->Visits)
            {
                best_child = child;
                best_score = child->WinPropability;
            }
        }
    }
    return best_child->Move.pos;
}

std::vector<MCTS::MoveInfo> MCTS::GetValidMoves(Node* target)
{
    RecreatePos(target);
    std::vector<MoveInfo> Result;
    if (!Evaluate().has_value())
    {
        for (int i = 0; i < m_GameState.GetDimensions(); i++)
        {
            for (int j = 0; j < m_GameState.GetDimensions(); j++)
            {
                if (m_GameState.GetAt(i, j) == 0)
                {
                    Result.emplace_back(i * MAP_SIZE + j, target->Move.player == 1 ? 2 : 1);
                }
            }
        }
    }
    return Result;
}

void MCTS::AddChildNode(Node* target, MoveInfo move)
{
    Node* New = new Node(0, 0, target, move);
    target->children.push_back(New);
    m_AllAlocations.push_back(New);
}

uint32_t MCTS::InRowCount(int posX, int posY, int dx, int dy) const
{
    uint32_t HitCount = 0;
    int Player = m_GameState.GetAt(posY, posX);
    while (m_GameState.GetAt(posY, posX) == Player)
    {
        HitCount++;
        if (HitCount >= WIN_CONDITION)
        {
            break;
        }
        if (posY + dy >= MAP_SIZE || posX + dx >= MAP_SIZE || posX + dx < 0 || posY + dy < 0)
        {
            break;
        }
        posY += dy;
        posX += dx;
    }
    return HitCount;
}
